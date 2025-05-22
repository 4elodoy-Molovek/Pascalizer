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
	

	// tmp
	PROGRAMM_END,

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
	SECTION,
	PROGRAM_END,
	WRONG,
	NULL_TOKEN, // this is not for you

	// comparison
	MORE_EQUAL,
	LESS,
	MORE,
	LESS_EQUAL,
	EQUAL,
	NOT_EQUAL
};

struct Token
{
	// Element's token - type
	TokenType type;

	// Element's value
	std::string value;	
};
