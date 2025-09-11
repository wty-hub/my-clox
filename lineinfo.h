#ifndef clox_lineinfo_h
#define clox_lineinfo_h

#include "common.h"
#include <stdint.h>
// challenge 14.1

typedef struct {
  int line;
  int count;
} LineInfo;

typedef struct {
  int count;
  int capacity;
  LineInfo* infos;
} LineInfoArray;

void initLineInfoArray(LineInfoArray* array);
void writeLineInfoArray(LineInfoArray* array, int line);
void freeLineInfoArray(LineInfoArray* array);
int getLine(LineInfoArray* array, int offset);

void printLineInfo(LineInfo lineInfo);
#endif