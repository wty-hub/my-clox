#include "chunk.h"
#include "memory.h"
#include "value.h"
#include "common.h"
#include <limits.h>
#include <stdlib.h>

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  initLineInfoArray(&chunk->lineInfos);
  initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    // chunk->lineInfo = GROW_ARRAY(LineInfo, chunk->lineInfo, oldCapacity, chunk->capacity);
  }
  chunk->code[chunk->count] = byte;
  writeLineInfoArray(&chunk->lineInfos, line);
  chunk->count++;
}

void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  freeLineInfoArray(&chunk->lineInfos);
  initChunk(chunk);
  freeValueArray(&chunk->constants);
}

void writeConstant(Chunk *chunk, Value value, int line) {
  int index = addConstant(chunk, value);
  if (index <= UINT8_MAX) {
    writeChunk(chunk, OP_CONSTANT, line);
    writeChunk(chunk, index, line);
  } else {
    writeChunk(chunk, (index >> 16) & 0xFF, line);
    writeChunk(chunk, (index >> 8) & 0xFF, line);
    writeChunk(chunk, index & 0xFF, line);
  }
}

int addConstant(Chunk *chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
