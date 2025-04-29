#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "ValuesTable.h"

/*
 * Program block, used by some instructions to implement expressions 
 */



/*
 * Base class for elements containing data about a single expression element 
 */
class ExpressionElement
{
public:
	ExpressionElement() {}
	virtual ~ExpressionElement() {}
};


/*
 * Expression element containing some value, that will be used in expression calculation
 */
class ValueExpressionElement final : public ExpressionElement
{
public:

	std::shared_ptr<Value> value;

	ValueExpressionElement(std::shared_ptr<Value> inValue): value(inValue) {}
	~ValueExpressionElement() {}
};

/*
 * Base class for an expression element containing some operation, that will be used in expression calculation
 */
class OperationExpressionElement : public ExpressionElement
{
public:

	OperationExpressionElement() {}
	virtual ~OperationExpressionElement() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ExpressionElement>>& calculationStack) = 0;
};



/*
 * Class, responbile for storing a postfix-written expression and calculating it's value
 */
class Expression
{
	// Postfix form of the expression
	std::vector<std::shared_ptr<ExpressionElement>> postfix;

public:
	Expression(const std::vector<std::shared_ptr<ExpressionElement>>& inPostfix) : postfix(inPostfix) {}

	// Calculates a result of the stored expression
	std::shared_ptr<Value> Caculate();
};



// OPERATIONS

class AddOperation final : public OperationExpressionElement
{
public:

	AddOperation() {}
	~AddOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ExpressionElement>>& calculationStack) override
	{
		if (auto rhs = dynamic_cast<ValueExpressionElement*>(calculationStack.top().get()))
		{
			calculationStack.pop();
			if (auto lhs = dynamic_cast<ValueExpressionElement*>(calculationStack.top().get()))
			{
				calculationStack.pop();

				// Calculation itself
				calculationStack.push(std::make_shared<ValueExpressionElement>(  *(rhs->value) + *(lhs->value)  ));
			}
		}
	}
};

// TO DO: Add more