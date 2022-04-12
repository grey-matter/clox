#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

// TODO Implement flexible array members: https://en.wikipedia.org/wiki/Flexible_array_member

typedef enum {
    OBJ_STRING,
} ObjType;

struct Obj {
    ObjType type;
    Obj* next;
};

struct ObjString {
    Obj obj;
    int length;
    char* chars;
};

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#define OBJ_TYPE(value)        (AS_OBJ(value)->type)
#define IS_STRING(value)       isObjType(value, OBJ_STRING)
#define AS_STRING(value)      ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)      (((ObjString*)AS_OBJ(value))->chars)

ObjString* copyString(const char* start, int length);
void printObject(Value val);
ObjString* takeString(char* chars, int length);

#endif