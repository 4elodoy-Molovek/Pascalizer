#pragma once
#include <string>

// All tokens: lexem types, present in the program
enum TokenType
{
	// Basic
	NAME,
	VALUE,
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
	CONST,
	VAR,
	PROGRAM_END
};

struct Token
{
	// Element's token - type
	TokenType type;

	// Element's value
	std::string value;
};
