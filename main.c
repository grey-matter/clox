#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

void testVM() {
	initVM();
	Chunk chunk;
	initChunk(&chunk);

	int address = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, address);

	address = addConstant(&chunk, 1.4);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, address);

	address = addConstant(&chunk, 1.6);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, address);

	address = addConstant(&chunk, 1.8);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, address);

	writeChunk(&chunk, OP_NEGATE);
	writeChunk(&chunk, OP_ADD);
	writeChunk(&chunk, OP_MULTIPLY);
	writeChunk(&chunk, OP_DIVIDE);
	writeChunk(&chunk, OP_RETURN);

	interpret(&chunk);
	freeVM();
}

int main(int argc, const char* argv[]) {
	testVM();
	return 0;
}