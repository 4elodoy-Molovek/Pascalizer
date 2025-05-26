#pragma once
#include <vector>
#include "Tokens.h"
#include "stdexcept"


/*
 * Program module, responsible for:
 *		- Separating source code into tokenized elements, that will later be processed by the Analysis Machine
 */
using namespace std;

enum STATES { ZERO_STATE, WORD_STATE, COLON_STATE, ASSIGN_STATE, QUOTES_STATE, CONST_STATE, NOT_EQUAL_STATE, MORE_STATE, LESS_STATE, EQUAL_STATE, EXCLAMATION_STATE };

class Tokenizer
{

	/* Tokenizer stores it's results into this cache field, returning only a const reference
	 * This allows us to avoid unnecessary copying later on
	 */
	std::vector<Token> cachedTokens;

private:

	Token identifyName(const string& name) 
	{
		Token token;

		while (true) 
		{
			// brgin end
			if (name == "begin") {
				token = { TokenType::BEGIN, "begin" };
				break;
			}
			if (name == "end") {
				token = { TokenType::END, "end" };
				break;
			}

			// specefic
			if (name == "program") {
				token = { TokenType::PROGRAM, "program" };
				break;
			}
			if (name == "var") {
				token = { TokenType::VAR, "var" };
				break;
			}
			if (name == "const") {
				token = { TokenType::CONST, "const" };
				break;
			}
			if (name == "end.") {
				cachedTokens.push_back(Token{ TokenType::END, "end" }); //! íó ýòî êàïåö êàêîé òî
				token = { TokenType::PROGRAMM_END, "." };
				break;
			}

			// if, then, else
			if (name == "if") {
				token = { TokenType::IF, "if" };
				break;
			}
			if (name == "then") {
				token = { TokenType::THEN, "then" };
				break;
			}
			if (name == "else") {
				token = { TokenType::ELSE, "else" };
				break;
			}
			if (name == "while") {
				token = { TokenType::WHILE, "while" };
				break;
			}

			// math
			if (name == "div") {
				token = { TokenType::MATH_OPERATOR, "/" }; //! êàêîé ó íåãî div
				break;
			}
			if (name == "mod") {
				token = { TokenType::MATH_OPERATOR, "%" }; //! êàêîé ó íåãî mod
				break;
			}

			token = { TokenType::NAME, name };
			break;
		}
		return token;
	}

	Token identifySymbol(const char& let, string& buf_name, int& state) 
	{
		Token tokElemnet{ TokenType::NULL_TOKEN, " " };
		while (true) {
			if (('a' <= let && let <= 'z') || ('A' <= let && let <= 'Z')) {
				buf_name += let;
				state = WORD_STATE;

				break;
			}
			if (let == '"') {
				//tokElemnet = { TokenType::QUOTES, "\"" };
				state = QUOTES_STATE;
				break;
			}
			if (let == ':') {
				state = COLON_STATE;
				break;
			}
			if (let == '!') {
				state = EXCLAMATION_STATE;
				break;
			}
			if ('0' <= let && let <= '9') {
				state = CONST_STATE;
				buf_name += let;
				break;
			}
			if (let == '<') {
				state = LESS_STATE;
				break;
			}
			if (let == '>') {
				state = MORE_STATE;
				break;
			}
			if (let == '=') {
				state = EQUAL_STATE;
				break;
			}

			switch (let) {
			case '\n': case ' ': case '\t':
				break;
			case '(':
				tokElemnet = { TokenType::BRACKET_OPEN, "(" };
				break;
			case ')':
				tokElemnet = { TokenType::BRACKET_CLOSE, ")" };
				break;
			case ';':
				tokElemnet = { TokenType::END_LINE, ";" };
				break;
			case ',':
				tokElemnet = { TokenType::COMMA, "," };
				break;
			case '+':
			case '-':
			case '/':
			case '*':
				tokElemnet = { TokenType::MATH_OPERATOR, string(1, let) };
				break;
			default:
				tokElemnet = { TokenType::WRONG, string(1, WRONG_CHAR_SYMBOL) };
				//W++;
				break;
			}
			break;
		}
		return tokElemnet;
	}

public:

	Tokenizer() = default;
	~Tokenizer() = default;

	// Tokenizes source code, splitting it into elements
	const std::vector<Token>& TokenizeCode(const std::string& sourceCode) 
	{
		string txt = sourceCode;
		string buf_name = "";
		int state = ZERO_STATE;
		Token tmpTok{ TokenType::NULL_TOKEN, " " };
		cachedTokens.clear();

		for (auto let : txt) {
			switch (state) {
			case ZERO_STATE:
			{
				if (let == '.')
				{
					buf_name += let;
					cachedTokens.push_back(identifyName(buf_name));
					buf_name.clear();
					state = ZERO_STATE;
					break;
				}

				tmpTok = identifySymbol(let, buf_name, state);
				if (tmpTok.type != TokenType::NULL_TOKEN)
					cachedTokens.push_back(tmpTok);
				break;
			}

			case CONST_STATE:
			{
				if ('0' <= let && let <= '9' || let == '.') {
					buf_name += let;
					break;
				}
				else {
					state = ZERO_STATE;

					int tmp_point_number = 0;
					for (char i : buf_name) {
						tmp_point_number += (i == '.');
					}

					if (tmp_point_number > 1)
						cachedTokens.push_back(Token{ TokenType::WRONG, string(1, WRONG_CHAR_SYMBOL) });
					if (tmp_point_number == 1)
						cachedTokens.push_back(Token{ TokenType::VALUE_DOUBLE, buf_name });
					if (tmp_point_number == 0)
						cachedTokens.push_back(Token{ TokenType::VALUE_INT, buf_name });

					buf_name.clear();

					// for the symbol
					tmpTok = identifySymbol(let, buf_name, state);
					if (tmpTok.type != TokenType::NULL_TOKEN)
						cachedTokens.push_back(tmpTok);
					break;
				}
				break;
			}
			case EXCLAMATION_STATE:
				if (let == '=') {
					cachedTokens.push_back(Token{ TokenType::NOT_EQUAL, "!=" });
					state = ZERO_STATE;
				}
				else {
					cachedTokens.push_back(Token{ TokenType::WRONG, string(1, WRONG_CHAR_SYMBOL) });

					tmpTok = identifySymbol(let, buf_name, state);
					if (tmpTok.type != TokenType::NULL_TOKEN)
						cachedTokens.push_back(tmpTok);
					break;
				}
				break;
			case WORD_STATE:
			{
				if (('a' <= let && let <= 'z') || ('A' <= let && let <= 'Z') || ('0' <= let && let <= '9') || (let == '_')) {
					buf_name += let;
					//state = WORD_STATE;
				}
				else {
					if (let == '.' && buf_name == "end") {
						buf_name += let;
						break;
					}
					cachedTokens.push_back(identifyName(buf_name));
					state = ZERO_STATE;
					buf_name.clear();

					tmpTok = identifySymbol(let, buf_name, state);
					if (tmpTok.type != TokenType::NULL_TOKEN)
						cachedTokens.push_back(tmpTok);
					break;
				}
				break;
			}

			case QUOTES_STATE:
			{
				if (let == '"') {
					cachedTokens.push_back(Token{ TokenType::VALUE_STRING, buf_name });
					//cachedTokens.push_back(Token{ TokenType::QUOTES, "\"" });
					buf_name.clear();
					state = ZERO_STATE;
				}
				else {
					buf_name += let;
				}
				break;
			}

			case COLON_STATE:
			{
				if (let == ' ') continue;
				if (let == '=')
				{
					state = ZERO_STATE;
					cachedTokens.push_back(Token{ TokenType::ASSIGN_OPERATOR, ":=" });
				}
				else {
					cachedTokens.push_back(Token{ TokenType::COLON, ":" });

					tmpTok = identifySymbol(let, buf_name, state);
					if (tmpTok.type != TokenType::NULL_TOKEN)
						cachedTokens.push_back(tmpTok);
					break;
				}
				break;
			}

			case EQUAL_STATE:
			{
				state = ZERO_STATE;
				if (let == '>') {
					cachedTokens.push_back(Token{ TokenType::MORE_EQUAL, "=>" });
					break;
				}
				if (let == '<') {
					cachedTokens.push_back(Token{ TokenType::LESS_EQUAL, "=>" });
					break;
				}
				cachedTokens.push_back(Token{ TokenType::EQUAL, "=" });

				tmpTok = identifySymbol(let, buf_name, state);
				if (tmpTok.type != TokenType::NULL_TOKEN)
					cachedTokens.push_back(tmpTok);
				break;
			}

			case LESS_STATE:
			{
				state = ZERO_STATE;
				if (let == '>') {
					cachedTokens.push_back(Token{ TokenType::NOT_EQUAL, "!=" });
				}
				cachedTokens.push_back(Token{ TokenType::LESS, ">" });

				tmpTok = identifySymbol(let, buf_name, state);
				if (tmpTok.type != TokenType::NULL_TOKEN)
					cachedTokens.push_back(tmpTok);
				break;
			}

			case MORE_STATE:
			{
				state = ZERO_STATE;
				if (let == '<') {
					cachedTokens.push_back(Token{ TokenType::NOT_EQUAL, "!=" });
				}
				cachedTokens.push_back(Token{ TokenType::MORE, "<" });

				tmpTok = identifySymbol(let, buf_name, state);
				if (tmpTok.type != TokenType::NULL_TOKEN)
					cachedTokens.push_back(tmpTok);
				break;
			}

			default:
				throw runtime_error("wrong state in Tokenizer");
				break;
			} // switch end


		} // for end

		if (buf_name.size() != 0) {
			cachedTokens.push_back(identifyName(buf_name));
			buf_name.clear();
		}
		return cachedTokens;
	}

	//Returns a reference to an indexed tokenized element //! ÿ ýòî íå ÷åêàë
	const Token& GetToken(size_t index) const 
	{
		return cachedTokens[index];
	}

	size_t GetTokenNumber() const { return cachedTokens.size(); }
};

