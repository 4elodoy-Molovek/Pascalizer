#pragma once
#include <string>
#include <map>

const char WRONG_CHAR_SYMBOL = ' ';

// All tokens: lexem types, present in the program
enum TokenType
{
	// Basic
	NAME,
	VALUE_INT,
	VALUE_DOUBLE,
	VALUE_STRING,
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
	// Element's type
	TokenType type;

	// Element's value
	std::string value;	
};


class TokenNames
{
public:
	static std::string GetTokenTypeName(TokenType type)
	{
		std::map<TokenType, std::string> names = 
		{
			{NAME, "NAME"},
			{VALUE_INT, "VALUE_INT"},
			{VALUE_DOUBLE, "VALUE_DOUBLE"},
			{VALUE_STRING, "VALUE_STRING"},
			{MATH_OPERATOR, "MATH_OPERATOR"},
			{ASSIGN_OPERATOR, "ASSIGN_OPERATOR"},
			{BRACKET_OPEN, "BRACKET_OPEN"},
			{BRACKET_CLOSE, "BRACKET_CLOSE"},
			{QUOTES, "QUOTES"},
			{COMMA, "COMMA"},
			{COLON, "COLON"},
			{END_LINE, "END_LINE"},
			{PROGRAMM_END, "PROGRAMM_END"},
			{IF, "IF"},
			{THEN, "THEN"},
			{ELSE, "ELSE"},
			{WHILE, "WHILE"},
			{BEGIN, "BEGIN"},
			{END, "END"},
			{VAR, "VAR"},
			{CONST, "CONST"},
			{PROGRAM, "PROGRAM"},
			{SECTION, "SECTION"},
			{PROGRAM_END, "PROGRAM_END"},
			{WRONG, "WRONG"},
			{NULL_TOKEN, "NULL_TOKEN"},
			{MORE_EQUAL, "MORE_EQUAL"},
			{LESS, "LESS"},
			{MORE, "MORE"},
			{LESS_EQUAL, "LESS_EQUAL"},
			{EQUAL, "EQUAL"},
			{NOT_EQUAL, "NOT_EQUAL"}
		};

		return names[type];
	}
};