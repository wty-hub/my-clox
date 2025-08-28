#include "lineinfo.h"
#include "common.h"
#include "memory.h"
#include <assert.h>
#include <stdio.h>

void initLineInfoArray(LineInfoArray *array) {
  array->capacity = 0;
  array->count = 0;
  array->infos = NULL;
}

void writeLineInfoArray(LineInfoArray *array, int line) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->infos =
        GROW_ARRAY(LineInfo, array->infos, oldCapacity, array->capacity);
  }
  if (array->count == 0) {
    array->count++;
    array->infos[0].count = 1;
    array->infos[0].line = line;
    return;
  }
  int curMaxLine = array->infos[array->count - 1].line;
  if (curMaxLine == line) {
    array->infos[array->count - 1].count++;
  } else if (curMaxLine < line) {
    array->infos[array->count].count = 1;
    array->infos[array->count].line = line;
    array->count++;
  } else {
    // line < curMaxLine, which is invalid
    assert(false && "Line numbers must be in ascending order");
  }
}

void freeLineInfoArray(LineInfoArray *array) {
  FREE_ARRAY(LineInfo, array->infos, array->capacity);
  initLineInfoArray(array);
}

int getLine(LineInfoArray *array, int offset) {
  int curOffset = 0;
  int i = 0;
  while (curOffset <= offset && i < array->count) {
    curOffset += array->infos[i].count;
    i += 1;
  }
  if (curOffset > offset) {
    return array->infos[i - 1].line;
  } else {
    assert(false && "invalid offset");
  }
}

void printLineInfo(LineInfo lineInfo) {
  printf("(%d, %d)", lineInfo.line, lineInfo.count);
}
