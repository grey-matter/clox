#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"

static Obj* allocateObject(size_t sz, ObjType objType) {
    Obj* obj = (Obj*)reallocate(NULL, 0, sz);
    obj->type = objType;
    return obj;
}

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

static ObjString* allocateString(char* chars, int length) {
    ObjString* obj = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    obj->length = length;
    obj->chars = chars;
    return obj;
}

ObjString* copyString(const char* chars, int length) {
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length);
}

ObjString* takeString(char* chars, int length) {
    return allocateString(chars, length);
}

void printObject(Value val) {
    switch (OBJ_TYPE(val)) {
        case OBJ_STRING:
            printf("%s\n", AS_CSTRING(val));
            break;
    }
}

