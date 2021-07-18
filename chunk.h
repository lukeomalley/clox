#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum
{
	OP_RETURN,
	OP_CONSTANT
} OpCode;

/** Line stores a run length encoded version of the line numbers for each bytecode instruction */
typedef struct
{
	int number;
	int count;
} Line;

/** Chunk is a dynamic array of bytecode instructions */
typedef struct
{
	int count;
	int lineCount; // number of unique source code lines within the chunk
	int capacity;
	uint8_t *code;
	Line *lines;
	ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int addConstant(Chunk *chunk, Value value);

Line getLine(Chunk *chunk, int chunkIndex);
void setLineNumber(Chunk *chunk, int lineNumber);

#endif