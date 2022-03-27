#include <stdio.h>
#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
	printf("== %s ==\n", name);
	for (int offset = 0; offset < chunk->count; ) {
		offset = disassembleInstruction(chunk, offset);
	}
}

int simpleInstruction(const char* name, int offset) {
	printf("%s\n", name);
	return offset + 1;
}

int constantInstruction(const char* name, Chunk* chunk, int offset) {
	uint8_t address = chunk->code[offset + 1];
	printf("%s %d ", name, address);
	printValue(chunk->constants.values[address]);
	return offset + 2;
}

int disassembleInstruction(Chunk* chunk, int offset) {
	uint8_t instruction = chunk->code[offset];
	printf("%04d ", offset);
	switch(instruction) {
		case OP_CONSTANT:
			return constantInstruction("OP_CONSTANT", chunk, offset);
			break;
		case OP_RETURN: 
			return simpleInstruction("OP_RETURN", offset);
			break;
		case OP_ADD:
			return simpleInstruction("OP_ADD", offset);
			break;
		case OP_SUBTRACT:
			return simpleInstruction("OP_SUBTRACT", offset);
			break;
		case OP_MULTIPLY:
			return simpleInstruction("OP_MULTIPLY", offset);
			break;
		case OP_DIVIDE:
			return simpleInstruction("OP_DIVIDE", offset);
			break;
		case OP_NEGATE:
			return simpleInstruction("OP_NEGATE", offset);
			break;
        case OP_NOT:
            return simpleInstruction("OP_NOT", offset);
            break;
        case OP_EQUALS:
            return simpleInstruction("OP_EQUALS", offset);
            break;
		default:
			printf("Unknown opcode %d\n", instruction);
			return offset + 1;
	}
}
