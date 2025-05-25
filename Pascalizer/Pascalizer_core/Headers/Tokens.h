#pragma once
#include <string>

const char WRONG_CHAR_SYMBOL = ' ';

// All tokens: lexem types, present in the program
enum TokenType
{
	// Basic
	NAME,
	INT_VALUE,
	DOUBLE_VALUE,
	STRING_VALUE,
	MATH_OPERATOR,
	ASSIGN_OPERATOR,
	BRACKET_OPEN,
	BRACKET_CLOSE,
	QUOTES,
	COMMA,
	COLON,
	END_LINE,

	// Condition
	IF,
	THEN,
	ELSE,
	WHILE,

	// Blocks
	BEGIN,
	END,
	VAR,
	CONST,

	// Specific
	PROGRAM,
	PROGRAMM_END,
	WRONG,
	NULL_TOKEN, // technical

	// comparison
	MORE_EQUAL,
	LESS,
	MORE,
	LESS_EQUAL,
	EQUAL,
	NOT_EQUAL,

	// logical operations
	OR,
	AND,
	NOT
};

struct Token
{
	// Element's token - type
	TokenType type;

	// Element's value
	std::string value;
};
