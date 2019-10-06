#include <stdio.h>
#include "vm.h"
#include "common.h"
#include "debug.h"

VM vm;

void resetStack() {
	vm.stackTop = vm.stack;
}

void initVM() {
	resetStack();
}

void freeVM() {}

static InterpretResult run() {
	#define READ_BYTE() (*vm.ip++)
	#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
	#define BINARY_OP(op) \
			do { \
				Value b = pop(); \
				Value a = pop(); \
				push(a op b); \
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
			case OP_ADD:
				BINARY_OP(+);
				break;
			case OP_SUBTRACT:
				BINARY_OP(-);
				break;
			case OP_MULTIPLY:
				BINARY_OP(*);
				break;
			case OP_DIVIDE:
				BINARY_OP(/);
				break;
			case OP_NEGATE:
				push(-pop());
				break;
			default:
				return INTERPRET_RUNTIME_ERROR; 
		}
	}

	#undef BINARY_OP
	#undef READ_CONSTANT
	#undef READ_BYTE
}

InterpretResult interpret(Chunk* chunk) {
	vm.chunk = chunk;
	vm.ip = chunk->code;
	return run();
}

void push(Value val) {
	*vm.stackTop = val;
	vm.stackTop++;
}

Value pop() {
	vm.stackTop--;
	return *vm.stackTop;
}