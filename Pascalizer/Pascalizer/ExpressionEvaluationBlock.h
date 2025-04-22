#pragma once
#include <string>
#include <vector>
#include <map>
#include "ValuesTable.h"

/*
 * Program block, used by some instructions to implement expressions 
 */


// Type of the expression element
enum ExpressionElementType {VALUE, STATIC_VALUE, OPERATION, FUNCTION};

// All possible operations in the expression
enum ExpressionOperation {PLUS, MINUS, DIVISION, MULTIPLICATION, MOD, EQUALS, NOT_EQUALS};


/*
 * Struct, containing data about a single expression element 
 */
struct ExpressionElement
{
	// Type of the element
	ExpressionElementType type;

	// Element itself, might be a String, Value or an ExpressionOperation
	// TO BE REPLACED BY SOME C++ SHENANIGANS
	void* element;
};


/*
 * Class, responbile for storing a postfix-written expression and calculating it's value
 */
class Expression
{
	// Postfix form of the expression
	std::vector<ExpressionElement> postfix;

public:
	Expression(const std::vector<ExpressionElement>& inPostfix) : postfix(inPostfix) {}

	// Calculates a result of the stored expression
	Value Caculate();
};