#include "value.h"
#include "memory.h"

void initValueArray(ValueArray* array) {
	array->count = array->capacity = 0;
	array->values = NULL;
}

void writeValueArray(ValueArray* array, Value val) {
	if (array->capacity < array->count + 1) {
		int oldCapacity = array->capacity;
		array->capacity = GROW_CAPACITY(oldCapacity);
		array->values = GROW_ARRAY(array->values, Value, oldCapacity, array->capacity);
	}
	array->values[array->count] = val;
	array->count++;
}

void freeValueArray(ValueArray* array) {
	FREE_ARRAY(Value, array->values, array->capacity);
	initValueArray(array);
}

void printValue(Value val) {
	printf("%g\n", val);
}