#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct
{
	const char *start;
	const char *current;
	int line;
} Scanner;

Scanner scanner;

/* Initialize the scanner to default values. Called before starting to scan a new source. */
void initScanner(const char *source)
{
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

/* isDigit checks if the next character is a digit 0 - 9 */
static bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}

/* Check if the current character is a string termination character */
static bool isAtEnd()
{
	return *scanner.current == '\0';
}

/* advance moves the current pointer ahead one and returns the next character in the source */
static char advance()
{
	scanner.current++;
	return scanner.current[-1];
}

/*
 * Attempt to match the next character to the expected if a match is found 
 * than we increment the current pointer to move onto the next character
 */
static bool match(char expected)
{
	if (isAtEnd())
	{
		return false;
	}

	if (*scanner.current != expected)
	{
		return false;
	}

	scanner.current++;
	return true;
}

/* Returns the next character in the source string */
static char peek()
{
	return *scanner.current;
}

/* Returns the character one after the next character in the source string */
static char peekNext()
{
	if (isAtEnd())
	{
		return '\0';
	}

	return scanner.current[1];
}

/* makeToken constructs a new token struct given a type */
static Token makeToken(TokenType type)
{
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	token.line = scanner.line;
	return token;
}

/* errorToken constructs an error token struct and stores error message within the token */
static Token errorToken(const char *message)
{
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;
	return token;
}

/*
* skipWhitespace consumes all of the next whitespace chatacters in the source
* string up until the next non whitespace character
*/
static void skipWhitespace()
{
	for (;;)
	{
		char c = peek();
		switch (c)
		{
		case ' ':
		case '\t':
		case '\r':
			advance();
			break;
		case '\n':
			scanner.line++;
			advance();
			break;
		case '/':
			if (peekNext() == '/')
			{
				while (peek() != '\n' && !isAtEnd())
				{
					advance();
				}
			}
			else
			{
				return;
			}

		default:
			return;
		}
	}
}

/* number parses the next number literal in the source */
static Token number()
{
	while (isDigit(peek()))
	{
		advance();
	}

	if (peek() == '.' && isDigit(peekNext()))
	{
		advance();

		while (isDigit(peek()))
		{
			advance();
		}
	}

	return makeToken(TOKEN_NUMBER);
}

/* string parses the next string literal in the source */
static Token string()
{
	while (peek() != '"' && !isAtEnd())
	{
		if (peek() == '\n')
		{
			scanner.line++;
		}
		advance();
	}

	if (isAtEnd())
	{
		return errorToken("Unterminated string.");
	}

	// "eat" the closing quote
	advance();
	return makeToken(TOKEN_STRING);
}

/* scanToken rethrns the next token in the source string */
Token scanToken()
{

	skipWhitespace();
	scanner.start = scanner.current;

	if (isAtEnd())
	{
		return makeToken(TOKEN_EOF);
	}

	char c = advance();

	if (isDigit(c))
	{
		return number();
	}

	switch (c)
	{
	case '(':
		return makeToken(TOKEN_LEFT_PAREN);
	case ')':
		return makeToken(TOKEN_RIGHT_PAREN);
	case '{':
		return makeToken(TOKEN_LEFT_BRACE);
	case '}':
		return makeToken(TOKEN_RIGHT_BRACE);
	case ';':
		return makeToken(TOKEN_SEMICOLON);
	case ',':
		return makeToken(TOKEN_COMMA);
	case '.':
		return makeToken(TOKEN_DOT);
	case '-':
		return makeToken(TOKEN_MINUS);
	case '+':
		return makeToken(TOKEN_PLUS);
	case '/':
		return makeToken(TOKEN_SLASH);
	case '*':
		return makeToken(TOKEN_STAR);
	case '!':
		return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
	case '=':
		return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
	case '<':
		return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
	case '>':
		return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
	case '"':
		return string();
	}

	return errorToken("Unexpected character.");
}