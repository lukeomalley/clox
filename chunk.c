#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

void initChunk(Chunk *chunk)
{
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->lineCount = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk)
{
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int lineNumber)
{
  if (chunk->capacity < chunk->count + 1)
  {
    // Allocate a new array with double the capacity
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(Line, chunk->lines, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;

  setLineNumber(chunk, lineNumber);

  chunk->count++;
}

int addConstant(Chunk *chunk, Value value)
{
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

Line getLine(Chunk *chunk, int byteIndex)
{
  int byteCount = 0;
  int i = 0;
  while (byteCount < byteIndex)
  {
    byteCount += chunk->lines[i].count;
    i++;
  }

  return chunk->lines[i];
}

/** 
 * Set the linenumber of the bytecode instruction within the chunk.
 * If bytecode is emitted from the same line as the last bytecode then a new line
 * is not allocated instead the count of the previous is incremented (run length encoding).
*/
void setLineNumber(Chunk *chunk, int lineNumber)
{
  Line *previousLine = &chunk->lines[chunk->lineCount];
  if (lineNumber == previousLine->number)
  {
    previousLine->count++;
  }
  else
  {
    Line line;
    line.count = 1;
    line.number = lineNumber;
    chunk->lines[chunk->count] = line;
    chunk->lineCount++;
  }
}