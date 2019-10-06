#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
	initVM();
	Chunk chunk;
	initChunk(&chunk);
	
	int address = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, address);

	writeChunk(&chunk, OP_RETURN);
	address = addConstant(&chunk, 1.4);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, address);

	// disassembleChunk(&chunk, "=====disassembling=====");
	interpret(&chunk);
	freeVM();
	return 0;
}