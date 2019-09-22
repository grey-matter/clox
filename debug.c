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
		default:
			printf("Unknown opcode %d\n", instruction);
			return offset + 1;
	}
}
