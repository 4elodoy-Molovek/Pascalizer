#pragma once
#include <string>

// All tokens: lexem types, present in the program
enum Token
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

struct TokenizedElement
{
	// Element's token - type
	Token token;

	// Element's value
	std::string value;
};
