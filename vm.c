#include <stdio.h>
#include "vm.h"
#include "common.h"
#include "debug.h"

VM vm;

void initVM() {}
void freeVM() {}

static InterpretResult run() {
	#define READ_BYTE() (*vm.ip++)
	#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
	while (1) {

		#ifdef DEBUG_TRACE_EXECUTION
			disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk->code));
		#endif
		
		uint8_t instruction = READ_BYTE();
		switch (instruction) {
			case OP_CONSTANT: {
				Value constant = READ_CONSTANT();
				printValue(constant);
				printf("\n");
				break;
			}
			case OP_RETURN:
				return INTERPRET_OK;
				break;
			default:
				return INTERPRET_RUNTIME_ERROR; 
		}
	}

	#undef READ_CONSTANT
	#undef READ_BYTE
}

InterpretResult interpret(Chunk* chunk) {
	vm.chunk = chunk;
	vm.ip = chunk->code;
	return run();
}