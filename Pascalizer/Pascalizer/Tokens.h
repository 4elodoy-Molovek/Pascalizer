#pragma once
#include <string>

// All token types: lexem types, present in the program
enum TokenType
{
	// Basic
	NAME,
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

	// Blocks
	BEGIN,
	END,

	// Specific
	PROGRAM,
	SECTION,
	PROGRAM_END
};

struct Token
{
	// Element's token type
	TokenType type;

	// Element's value
	std::string value;
};
