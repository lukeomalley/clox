#include <stdio.h>
#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

int main(int argc, const char *argv[])
{

	initVM();

	Chunk chunk;
	initChunk(&chunk);

	int constant = addConstant(&chunk, 3);
	writeChunk(&chunk, OP_CONSTANT, 123);
	writeChunk(&chunk, constant, 123);

	int constant_2 = addConstant(&chunk, 3);
	writeChunk(&chunk, OP_CONSTANT, 124);
	writeChunk(&chunk, constant_2, 124);

	writeChunk(&chunk, OP_MULTIPLY, 125);

	int constant_3 = addConstant(&chunk, 3);
	writeChunk(&chunk, OP_CONSTANT, 125);
	writeChunk(&chunk, constant_3, 125);

	writeChunk(&chunk, OP_DIVIDE, 125);

	writeChunk(&chunk, OP_NEGATE, 125);
	writeChunk(&chunk, OP_RETURN, 125);

	interpret(&chunk);
	freeVM();
	freeChunk(&chunk);
	return 0;
}