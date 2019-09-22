#ifndef clox_value_h
#define clox_value_h
#include "common.h"
typedef double Value;

typedef struct {
	int count, capacity;
	Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value val);
void freeValueArray(ValueArray* array);
void printValue(Value val);

#endif
