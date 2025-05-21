#pragma once
#include <vector>
#include "Tokens.h"
#include "stdexcept"


/*
 * Program module, responsible for:
 *		- Separating source code into tokenized elements, that will later be processed by the Analysis Machine
 */
using namespace std;

enum STATES { ZERO_STATE, WORD_STATE, COLON_STATE, ASSIGN_STATE, QUOTES_STATE, CONST_STATE, NOT_EQUAL_STATE, MORE_STATE, LESS_STATE, EQUAL_STATE, EXCLAMATION_STATE};

class Tokenizer
{

	/* Tokenized stores it's results into this cache field, returning only a const reference
	 * This allows us to avoid unnecessary copying later on
	 */
	std::vector<TokenizedElement> cachedTokens;

private:

	TokenizedElement identifyName(const string& name) {
		TokenizedElement token;
		
		while (true) {
			// brgin end
			if (name == "begin") {
				 token = { Token::BEGIN, "begin" };
				break;
			}
			if (name == "end") {
				token = { Token::END, "end" };
				break;
			}

			// specefic
			if (name == "program") {
				token = { Token::PROGRAM, "program" };
				break;
			}
			if (name == "var") {
				token = { Token::VAR, "var" };
				break;
			}
			if (name == "const") {
				token = { Token::CONST, "const" };
				break;
			}
			if (name == "end.") {
				cachedTokens.push_back(TokenizedElement{ Token::END, "end" }); //! ну это капец какой то
				token = { Token::PROGRAMM_END, "." }; 
				break;
			}

			// if, then, else
			if (name == "if") {
				token = { Token::IF, "if" };
				break;
			}
			if (name == "then") {
				token = { Token::THEN, "then" };
				break;
			}
			if (name == "else") {
				token = { Token::ELSE, "else" };
				break;
			}
			if (name == "while") {
				token = { Token::WHILE, "while" };
				break;
			}

			// math
			if (name == "div") {
				token = { Token::MATH_OPERATOR, "/" }; //! какой у него div
				break;
			}
			if (name == "mod") {
				token = { Token::MATH_OPERATOR, "%" }; //! какой у него mod
				break;
			}

			token = { Token::NAME, name }; 
			break;
		}
		return token;
	}

	TokenizedElement identifySymbol(const char& let, string& buf_name, int& state) {
		TokenizedElement tokElemnet{ Token::NULL_TOKEN, " " };
		while (true) {
			if (('a' <= let && let <= 'z') || ('A' <= let && let <= 'Z')) {
				buf_name += let;
				state = WORD_STATE;
				
				break;
			}
			if (let == '"') {
				tokElemnet = { Token::QUOTES, "\""};
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
				tokElemnet = { Token::BRACKET_OPEN, "(" };
				break;
			case ')':
				tokElemnet = { Token::BRACKET_CLOSE, ")" };
				break;
			case ';':
				tokElemnet = { Token::SEMICOLON, ";" };
				break;
			case ',':
				tokElemnet = { Token::COMMA, "," };
				break;
			case '+':
			case '-':
			case '/':
			case '*':
				tokElemnet = { Token::MATH_OPERATOR, string(1, let) };
				break;
			default:
				tokElemnet = { Token::WRONG, string(1, WRONG_CHAR_SYMBOL) };
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
	const std::vector<TokenizedElement>& TokenizeCode(const std::string& sourceCode) {
		string txt = sourceCode;
		string buf_name = "";
		int state = ZERO_STATE;
		TokenizedElement tmpTok { Token::NULL_TOKEN, " " };

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
				if (tmpTok.token != Token::NULL_TOKEN)
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
						cachedTokens.push_back(TokenizedElement{ Token::WRONG, string(1, WRONG_CHAR_SYMBOL) });
					else 
						cachedTokens.push_back(TokenizedElement{Token::VALUE, buf_name});
					buf_name.clear();
					
					tmpTok = identifySymbol(let, buf_name, state);
					if (tmpTok.token != Token::NULL_TOKEN)
						cachedTokens.push_back(tmpTok);
					break;
				}
				break;
			}
			case EXCLAMATION_STATE:
				if (let == '=') {
					cachedTokens.push_back(TokenizedElement{ Token::NOT_EQUAL, "!=" });
					state = ZERO_STATE;
				}
				else {
					cachedTokens.push_back(TokenizedElement{ Token::WRONG, string(1, WRONG_CHAR_SYMBOL) });
					
					tmpTok = identifySymbol(let, buf_name, state);
					if (tmpTok.token != Token::NULL_TOKEN)
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
					if (tmpTok.token != Token::NULL_TOKEN)
						cachedTokens.push_back(tmpTok);
					break;
				}
				break;
			}

			case QUOTES_STATE:
			{
				if (let == '"') {
					cachedTokens.push_back(TokenizedElement{ Token::VALUE, buf_name });
					cachedTokens.push_back(TokenizedElement{ Token::QUOTES, "\""});
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
					cachedTokens.push_back(TokenizedElement{ Token::ASSIGN_OPERATOR, ":=" });
				}
				else {
					cachedTokens.push_back(TokenizedElement{ Token::COLON, ":" });
					
					tmpTok = identifySymbol(let, buf_name, state);
					if (tmpTok.token != Token::NULL_TOKEN)
						cachedTokens.push_back(tmpTok);
					break;
				}
				break;
			}

			case EQUAL_STATE:
			{
				state = ZERO_STATE;
				if (let == '>') {
					cachedTokens.push_back(TokenizedElement{ Token::MORE_EQUAL, "=>" });
					break;
				}
				if (let == '<') {
					cachedTokens.push_back(TokenizedElement{ Token::LESS_EQUAL, "=>" });
					break;
				}
				cachedTokens.push_back(TokenizedElement{ Token::EQUAL, "=" });
				
				tmpTok = identifySymbol(let, buf_name, state);
				if (tmpTok.token != Token::NULL_TOKEN)
					cachedTokens.push_back(tmpTok);
				break;
			}

			case LESS_STATE:
			{
				state = ZERO_STATE;
				if (let == '>') {
					cachedTokens.push_back(TokenizedElement{ Token::NOT_EQUAL, "!=" });
				}
				cachedTokens.push_back(TokenizedElement{ Token::LESS, ">" });
				
				tmpTok = identifySymbol(let, buf_name, state);
				if (tmpTok.token != Token::NULL_TOKEN)
					cachedTokens.push_back(tmpTok);
				break;
			}

			case MORE_STATE:
			{
				state = ZERO_STATE;
				if (let == '<') {
					cachedTokens.push_back(TokenizedElement{ Token::NOT_EQUAL, "!=" });
				}
				cachedTokens.push_back(TokenizedElement{ Token::MORE, "<" });
				
				tmpTok = identifySymbol(let, buf_name, state);
				if (tmpTok.token != Token::NULL_TOKEN)
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

	 //Returns a reference to an indexed tokenized element //! я это не чекал
	const TokenizedElement& GetTokenizedElement(size_t index) const {
		return cachedTokens[index];
	}

	size_t GetTokenNumber() const { return cachedTokens.size(); }
};

	