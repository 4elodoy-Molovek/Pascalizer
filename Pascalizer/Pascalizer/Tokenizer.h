#pragma once
#include <vector>
#include "Tokens.h"

/*
 * Program module, responsible for:
 *		- Separating source code into tokens, that will later be processed by the Analysis Machine
 */
class Tokenizer
{

	/* Tokenizer stores it's results into this cache field, returning only a const reference
	 * This allows us to avoid unnecessary copying later on
	 */
	std::vector<Token> cachedTokens;

public: 

	Tokenizer();
	~Tokenizer();

	// Tokenizes source code, splitting it into elements
	const std::vector<Token>& TokenizeCode(const std::string& sourceCode);

	// Returns a reference to an indexed tokenized element
	const Token& GetTokens(size_t index) const;

	size_t GetTokenNumber() const { return cachedTokens.size(); }
};
