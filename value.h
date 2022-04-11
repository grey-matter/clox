#ifndef clox_value_h
#define clox_value_h
#include "common.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum {
  VAL_BOOL,
  VAL_NIL, 
  VAL_NUMBER,
  VAL_OBJ,
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Obj* obj;
    } as;
} Value;

typedef struct {
	int count, capacity;
	Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value val);
void freeValueArray(ValueArray* array);
void printValue(Value val);
bool valuesEqual(Value a, Value b);

#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_OBJ(value)     ((value).type == VAL_OBJ)

#define AS_NUMBER(value) ((value).as.number)
#define AS_BOOL(value) ((value).as.boolean)
#define AS_OBJ(value)     ((value).as.obj)

#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL ((Value){VAL_NIL, {.number = 0}})
#define OBJ_VAL(object)   ((Value){VAL_OBJ, {.obj = (Obj*)object}})

#endif
