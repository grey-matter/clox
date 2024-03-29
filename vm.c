#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "vm.h"
#include "common.h"
#include "debug.h"
#include "compiler.h"
#include "object.h"
#include "memory.h"

VM vm;

void resetStack() {
	vm.stackTop = vm.stack;
}

void initVM() {
	resetStack();
    vm.objects = NULL;
}

void freeVM() {
    freeObjects();
}

static Value peek(int ind) {
    return *(vm.stackTop - 1 - ind);
}

void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

    // fprintf(stderr, "")

}

static bool isFalsey(Value val) {
    return val.type == VAL_NIL || (val.type == VAL_BOOL && !val.as.boolean);
}

static void concatenate() {
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    int length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = takeString(chars, length);
    push(OBJ_VAL(result));
}

static InterpretResult run() {
	#define READ_BYTE() (*vm.ip++)
	#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
	#define BINARY_OP(valueType, op) \
			do { \
                if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
                    runtimeError("Expected number."); \
                    return INTERPRET_RUNTIME_ERROR; \
                } \
				double b = AS_NUMBER(pop()); \
				double a = AS_NUMBER(pop()); \
				push(valueType(a op b)); \
			} while (false)

	while (1) {

		#ifdef DEBUG_TRACE_EXECUTION
			for (Value* operand = vm.stack; operand < vm.stackTop; ++operand) {
				printf("[ ");
				printValue(*operand);
				printf(" ]");
			}
			printf("\n");
			disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk->code));
		#endif
		
		uint8_t instruction = READ_BYTE();
		switch (instruction) {
			case OP_CONSTANT: {
				Value constant = READ_CONSTANT();
				// printValue(constant);
				// printf("\n");
				push(constant);
				break;
			}
			case OP_RETURN:
				printValue(pop());
				printf("\n");
				return INTERPRET_OK;
				break;
			case OP_ADD: {
                if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                } else {
                    runtimeError(
                        "Operands must be two numbers or two strings.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
			case OP_SUBTRACT:
				BINARY_OP(NUMBER_VAL, -);
				break;
			case OP_MULTIPLY:
				BINARY_OP(NUMBER_VAL, *);
				break;
			case OP_DIVIDE:
				BINARY_OP(NUMBER_VAL, /);
				break;
			case OP_NEGATE:
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Expected a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
				push(NUMBER_VAL(-AS_NUMBER(pop())));
				break;
            case OP_NOT:
                push(BOOL_VAL(isFalsey(pop())));
                break; 
            case OP_EQUALS: \
                {
                    Value a = pop();
                    Value b = pop();
                    push(BOOL_VAL(valuesEqual(a, b)));
                }
                break;
			default:
				return INTERPRET_RUNTIME_ERROR; 
		}
	}

	#undef BINARY_OP
	#undef READ_CONSTANT
	#undef READ_BYTE
}

InterpretResult interpret(const char* source) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);
	return result;
}

void push(Value val) {
    *vm.stackTop = val;
	vm.stackTop++;
    
}

Value pop() {
	vm.stackTop--;
	return *vm.stackTop;
}