#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void repl() {
	char line[1024];
	while (1) {
		printf("> ");
		if (!fgets(line, sizeof(line), stdin)) {
			break;
		}
		interpret(line);
	}
}

char* readFile(const char* path) {
	FILE* file = fopen(path, "rb");
	
	if (!file) {
		fprint(stderr, "Could not open file: %s\n", path);
		exit(65);
	}

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char* buffer = (char*) malloc(fileSize + 1);
	if (!buffer) {
		fprintf(stderr, "Not enough memory to read.\n");
		exit(66);
	}
	
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	if (bytesRead < fileSize) {
		fprintf(stderr, "Could not read file.\n");
		exit(65);
	}
	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}

void runFile(const char* path) {
	char* source = readFile(path);
	interpret(source);
	free(source);
}

int main(int argc, const char* argv[]) {
	// testVM();
	if (argc == 1) {
		repl();
	} else if (argc == 2) {
		runFile(argv[1]);
	} else {
		fprintf(stderr, "usage: clox [path]\n");
		exit(64);
	}
	return 0;
}