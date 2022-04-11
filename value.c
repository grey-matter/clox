#include <string.h>

#include "object.h"
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
    switch(val.type) {
        case VAL_NUMBER:
            printf("%g\n", AS_NUMBER(val));
            break;
        case VAL_BOOL:
            printf("%s\n", AS_BOOL(val) ? "true" : "false");
            break;
        case VAL_OBJ:
            printObject(val);
            break;
    }
}

bool valuesEqual(Value a, Value b) {
    if (a.type != b.type) {
        return false;
    }
    switch (a.type) {
        case VAL_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
            break;
        case VAL_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
            break;
        case VAL_NIL:
            return true;
            break;
        case VAL_OBJ: {
            ObjString* aString = AS_STRING(a);
            ObjString* bString = AS_STRING(b);
            return aString->length == bString->length &&
                memcmp(aString->chars, bString->chars,
                        aString->length) == 0;
        }
    }
}
