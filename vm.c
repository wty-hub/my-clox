#include "vm.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "debug.h"
#include "memory.h"

VM vm;

static void resetStack() {
  vm.stack = (Value*)malloc(STACK_MAX * sizeof(Value));
  vm.stackTop = vm.stack;
  vm.stackCapacity = STACK_MAX;
}

void initVM() { resetStack(); }

void freeVM() { free(vm.stack); }

// Helper function to read a 3-byte constant index
static uint32_t readLongConstantIndex() {
  uint32_t byte1 = *(vm.ip++);
  uint32_t byte2 = *(vm.ip++);
  uint32_t byte3 = *(vm.ip++);
  return (byte1 << 16) | (byte2 << 8) | byte3;
}

static InterpretResult run() {
#define READ_BYTE() (*(vm.ip++))

#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

#define READ_LONG_CONSTANT() \
  (vm.chunk->constants.values[readLongConstantIndex()])
  if (vm.chunk->count == 0) return INTERPRET_OK;

#define BINARY_OP(op) \
  do {                \
    double b = pop(); \
    double a = pop(); \
    push(a op b);     \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    OpCode instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value constant = READ_CONSTANT();
        push(constant);
        printf("\n");
        break;
      }
      case OP_CONSTANT_LONG: {
        Value constant = READ_LONG_CONSTANT();
        push(constant);
        printf("\n");
        break;
      }
      case OP_RETURN: {
        printValue(pop());
        printf("\n");
        return INTERPRET_OK;
      }
      case OP_ADD:
        BINARY_OP(+);
        break;
      case OP_SUBTRACT:
        BINARY_OP(-);
        break;
      case OP_MULTIPLY:
        BINARY_OP(*);
        break;
      case OP_DIVIDE:
        BINARY_OP(/);
        break;
      case OP_NEGATE: {
        // push(-pop());
        *(vm.stackTop - 1) = -*(vm.stackTop - 1);
        break;
      }
      default:
        break;
    }
  }
#undef BINARY_OP
#undef READ_LONG_CONSTANT
#undef READ_CONSTANT
#undef READ_BYTE
}

InterpretResult interpret(const char* source) {
  Chunk chunk;
  initChunk(&chunk);
  if (!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILE_ERROR;
  }

  vm.chunk = &chunk;
  vm.ip = vm.chunk->code;

  InterpretResult result = run();

  freeChunk(&chunk);
  return result;
}

void push(Value value) {
  *vm.stackTop = value;
  vm.stackTop++;
  if ((vm.stackTop - vm.stack) / sizeof(vm.stack[0]) >= vm.stackCapacity) {
    size_t curCount = vm.stackTop - vm.stack;
    vm.stack =
        GROW_ARRAY(Value, vm.stack, vm.stackCapacity, vm.stackCapacity * 2);
    vm.stackCapacity = vm.stackCapacity * 2;
    vm.stackTop = vm.stack + curCount;
  }
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}
