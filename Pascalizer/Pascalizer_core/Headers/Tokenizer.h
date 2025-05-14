#pragma once
#include <vector>
#include "Tokens.h"

/*
 * Program module, responsible for:
 *		- Separating source code into tokenized elements, that will later be processed by the Analysis Machine
 */
class Tokenizer
{

	/* Tokenized stores it's results into this cache field, returning only a const reference
	 * This allows us to avoid unnecessary copying later on
	 */
	std::vector<TokenizedElement> cachedTokens;

public: 

	Tokenizer();
	~Tokenizer();

	// Tokenizes source code, splitting it into elements
	const std::vector<TokenizedElement>& TokenizeCode(const std::string& sourceCode);

	// Returns a reference to an indexed tokenized element
	const TokenizedElement& GetTokenizedElement(size_t index) const;

	size_t GetTokenNumber() const { return cachedTokens.size(); }
};