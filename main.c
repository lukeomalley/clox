#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "chunk.h"
#include "vm.h"
#include "debug.h"

static void repl()
{
	char line[1024];

	for (;;)
	{
		printf("> ");

		if (!fgets(line, sizeof(line), stdin))
		{
			printf("\n");
			break;
		}

		interpret(line);
	}
}

static char *readFile(const char *path)
{
	// Open the file
	FILE *file = fopen(path, "rb");
	if (file == NULL)
	{
		fprintf(stderr, "Could not open the file \"%s\".\n", path);
		exit(74);
	}

	// Get file length: seek to end of file, tell how far we are from the start of the file, then rewind to start
	fseek(file, 0L, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	// Allocate buffer to hold the file
	char *buffer = (char *)malloc(fileSize + 1);
	if (buffer == NULL)
	{
		fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
		exit(74);
	}

	// Read the file into the buffer
	size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
	if (bytesRead < fileSize)
	{
		fprintf(stderr, "Could not read file \"%s\".\n", path);
		exit(74);
	}

	// Add a null termination char at end of file
	buffer[bytesRead] = '\0';
	fclose(file);
	return buffer;
}

static void runFile(const char *path)
{
	char *source = readFile(path);
	InterpretResult result = interpret(source);
	free(source);

	if (result == INTERPRET_COMPILE_ERROR)
	{
		exit(65);
	}

	if (result == INTERPRET_RUNTIME_ERROR)
	{
		exit(70);
	}
}

int main(int argc, const char *argv[])
{
	initVM();

	if (argc == 1)
	{
		repl();
	}
	else if (argc == 2)
	{
		runFile(argv[1]);
	}
	else
	{
		fprintf(stderr, "Usage: clox [path]\n");
		exit(64);
	}

	freeVM();
	return 0;
}
