//
// Created by wty on 2025/9/22.
//

#ifndef MY_CLOX_TABLE_H
#define MY_CLOX_TABLE_H

#include "value.h"


typedef struct {
  ObjString* key;
  Value value;
} Entry;

typedef struct {
  int count;
  int capacity;
  Entry* entries;
} Table;

void initTable(Table* table);

void freeTable(Table* table);

bool tableSet(Table* table, ObjString* key, Value value);

#endif //MY_CLOX_TABLE_H