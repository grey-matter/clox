#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]) {
	Chunk chunk;
	initChunk(&chunk);
	
	int address = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, address);

	writeChunk(&chunk, OP_RETURN);
	address = addConstant(&chunk, 1.4);
	writeChunk(&chunk, OP_CONSTANT);
	writeChunk(&chunk, address);

	disassembleChunk(&chunk, "test chunk");
	freeChunk(&chunk);
	disassembleChunk(&chunk, "empty chunk");
	return 0;
}