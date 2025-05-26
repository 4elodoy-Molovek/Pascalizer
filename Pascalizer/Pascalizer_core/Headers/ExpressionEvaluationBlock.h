#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <stdexcept>
#include "ProgramState.h"
#include "ValuesTable.h"

/*
 * Program block, used by some instructions to implement expressions
 */


 // All posible operation types
enum OperationType
{
	OP_NONE, OP_ADD, OP_SUBTRACT, OP_DIVIDE, OP_MULTIPLY, OP_MOD, OP_DIV,
	OP_MORE, OP_EQUAL, OP_MORE_EQUAL, OP_LESS, OP_NOT_EQUAL, OP_LESS_EQUAL,
	OP_SIN
};

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

	ValueExpressionElement(std::shared_ptr<Value> inValue) : value(inValue) {}
	~ValueExpressionElement() {}
};

/*
 * Expression element containing a name of a variable, that will be used in expression calculation
 */
class VariableExpressionElement final : public ExpressionElement
{
public:

	std::string varName;

	VariableExpressionElement(std::string inVarName) : varName(inVarName) {}
	~VariableExpressionElement() {}
};

/*
 * Base class for an expression element containing some operation, that will be used in expression calculation
 */
class OperationExpressionElement : public ExpressionElement
{
protected:
	// Type of this operation
	OperationType type = OP_NONE;

public:

	const OperationType& GetType() { return type; }

	OperationExpressionElement() {}
	virtual ~OperationExpressionElement() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) = 0;
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
	std::shared_ptr<Value> Caculate(ProgramState& programState)
	{
		auto localPostfix = postfix;

		// Loading variables from the program state
		for (int i = 0; i < postfix.size(); i++)
		{
			if (auto varElement = dynamic_cast<VariableExpressionElement*>(localPostfix[i].get()))
			{
				// Checking if variable is valid
				if (programState.valuesTable.count(varElement->varName) == 0) throw(std::runtime_error("RUNTIME ERROR: Unknown variable '" + varElement->varName + "'!"));

				// Replacing variable element with a value element
				std::shared_ptr<Value> varValue = programState.valuesTable[varElement->varName];
				localPostfix[i] = std::make_shared<ValueExpressionElement>(varValue);
			}
		}

		std::stack<std::shared_ptr<ValueExpressionElement>> calculationStack;
		for (std::shared_ptr<ExpressionElement> el : localPostfix)
		{

			// if cur_elemnt is value
			if (auto tmp = dynamic_cast<ValueExpressionElement*>(el.get())) // type of tmp is ValueExpressionElement*
			{
				calculationStack.push(std::make_shared<ValueExpressionElement>(*tmp));
				continue;
			}

			// if cur_elemnt is operation
			if (auto tmp = dynamic_cast<OperationExpressionElement*>(el.get()))  // type of tmp is OperationExpressionElement*
			{
				// calculating
				tmp->Calculate(calculationStack); //? à ÷å áóäåò åñëè â åãî êëàññå íåò ïåðåîïðåäåëåíèÿ calculate()?
				continue;
			}

			throw(std::exception("Unrocognized type of ExpressionElement"));
		}
		return calculationStack.top()->value;
	}
};



// OPERATIONS

class AddOperation final : public OperationExpressionElement
{
public:

	AddOperation() { type = OP_ADD; }
	~AddOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop(); //? ÷òî âîçâðàòèò top/pop èç ïóñòîãî ñòåêà
		//auto rhs = rhs_.get();

		auto lhs = calculationStack.top(); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(rhs->value) + *(lhs->value)));
		//? ñëîæåíèå ïðèíèìàåò îáû÷íûå -ïîèíòåðû- ïåðåìåííûå, íî âîçâðàùàåò shared
	}
};


class MultiplyOperation final : public OperationExpressionElement
{
public:

	MultiplyOperation() { type = OP_MULTIPLY; }
	~MultiplyOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop(); //? ÷òî âîçâðàòèò top/pop èç ïóñòîãî ñòåêà
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(rhs->value) * *(lhs->value)));
		//? ñëîæåíèå ïðèíèìàåò îáû÷íûå -ïîèíòåðû- ïåðåìåííûå, íî âîçâðàùàåò shared
	}
};


class SubtractOperation final : public OperationExpressionElement
{
public:

	SubtractOperation() { type = OP_SUBTRACT; }
	~SubtractOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop(); //? ÷òî âîçâðàòèò top/pop èç ïóñòîãî ñòåêà
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(lhs->value) - *(rhs->value)));
		//? ñëîæåíèå ïðèíèìàåò îáû÷íûå -ïîèíòåðû- ïåðåìåííûå, íî âîçâðàùàåò shared
	}
};


class DivideOperation final : public OperationExpressionElement
{
public:

	DivideOperation() { type = OP_DIVIDE; }
	~DivideOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop(); //? ÷òî âîçâðàòèò top/pop èç ïóñòîãî ñòåêà
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(lhs->value) / *(rhs->value)));
		//? ñëîæåíèå ïðèíèìàåò îáû÷íûå -ïîèíòåðû- ïåðåìåííûå, íî âîçâðàùàåò shared
	}
};



class ModOperation final : public OperationExpressionElement
{
public:

	ModOperation() { type = OP_MOD; }
	~ModOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop(); //? ÷òî âîçâðàòèò top/pop èç ïóñòîãî ñòåêà
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(lhs->value) % *(rhs->value)));
		//? ñëîæåíèå ïðèíèìàåò îáû÷íûå -ïîèíòåðû- ïåðåìåííûå, íî âîçâðàùàåò shared
	}
};


class DivOperation final : public OperationExpressionElement
{
public:

	DivOperation() { type = OP_DIV; }
	~DivOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop();
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(div((*lhs->value), (*rhs->value))));
	}
};

class EqualOperation final : public OperationExpressionElement
{
public:

	EqualOperation() { type = OP_EQUAL; }
	~EqualOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop();
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>((*lhs->value) == (*rhs->value)));
	}
};

class NotEqualOperation final : public OperationExpressionElement
{
public:

	NotEqualOperation() { type = OP_NOT_EQUAL; }
	~NotEqualOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop();
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>((*lhs->value) != (*rhs->value)));
	}
};

class LessOperation final : public OperationExpressionElement
{
public:

	LessOperation() { type = OP_LESS; }
	~LessOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop();
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>((*lhs->value) < (*rhs->value)));
	}
};

class LessEqualOperation final : public OperationExpressionElement
{
public:

	LessEqualOperation() { type = OP_LESS_EQUAL; }
	~LessEqualOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop();
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>((*lhs->value) <= (*rhs->value)));
	}
};

class MoreOperation final : public OperationExpressionElement
{
public:

	MoreOperation() { type = OP_MORE; }
	~MoreOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop();
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>((*lhs->value) > (*rhs->value)));
	}
};

class MoreEqualOperation final : public OperationExpressionElement
{
public:

	MoreEqualOperation() { type = OP_MORE_EQUAL; }
	~MoreEqualOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop();
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>((*lhs->value) >= (*rhs->value)));
	}
};


class SinOperation final : public OperationExpressionElement
{
public:

	SinOperation() { type = OP_SIN; }
	~SinOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop(); //? ÷òî âîçâðàòèò top/pop èç ïóñòîãî ñòåêà
		calculationStack.push(std::make_shared<ValueExpressionElement>(usin(*(rhs->value))));
	}
};