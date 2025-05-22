#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <stdexcept>
#include "ValuesTable.h"

/*
 * Program block, used by some instructions to implement expressions 
 */


// All posible operation types
enum OperationType {OP_NONE, OP_ADD, OP_SUBTRACT, OP_DIVIDE, OP_MULTIPLY, OP_MOD, OP_DIV, OP_SIN /*...*/ };

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
		// Loading variables from the program state
		for (int i = 0; i < postfix.size(); i++)
		{
			if (auto varElement = dynamic_cast<VariableExpressionElement*>(postfix[i].get()))
			{
				// Checking if variable is valid
				if (programState.valuesTable.count(varElement->varName) == 0) throw(std::runtime_error("RUNTIME ERROR: Unknown variable '" + varElement->varName + "'!"));

				// Replacing variable element with a value element
				std::shared_ptr<Value> varValue = programState.valuesTable[varElement->varName];
				postfix[i] = std::make_shared<ExpressionElement>(ValueExpressionElement(varValue));
			}
		}


		std::stack<std::shared_ptr<ValueExpressionElement>> calculationStack;
		for (std::shared_ptr<ExpressionElement> el : postfix) 
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
				tmp->Calculate(calculationStack); //? а че будет если в его классе нет переопределения calculate()?
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
		auto rhs = calculationStack.top(); calculationStack.pop(); //? что возвратит top/pop из пустого стека
		//auto rhs = rhs_.get();

		auto lhs = calculationStack.top(); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(rhs->value) + *(lhs->value)));
		//? сложение принимает обычные -поинтеры- переменные, но возвращает shared
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
		auto rhs = calculationStack.top(); calculationStack.pop(); //? что возвратит top/pop из пустого стека
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(rhs->value) * *(lhs->value)));
		//? сложение принимает обычные -поинтеры- переменные, но возвращает shared
	}
};


class SubstractOperation final : public OperationExpressionElement
{
public:

	SubstractOperation() { type = OP_SUBTRACT; }
	~SubstractOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop(); //? что возвратит top/pop из пустого стека
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(lhs->value) - *(rhs->value)));
		//? сложение принимает обычные -поинтеры- переменные, но возвращает shared
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
		auto rhs = calculationStack.top(); calculationStack.pop(); //? что возвратит top/pop из пустого стека
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(lhs->value) / *(rhs->value)));
		//? сложение принимает обычные -поинтеры- переменные, но возвращает shared
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
		auto rhs = calculationStack.top(); calculationStack.pop(); //? что возвратит top/pop из пустого стека
		auto lhs = (calculationStack.top()); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(lhs->value) % *(rhs->value)));
		//? сложение принимает обычные -поинтеры- переменные, но возвращает shared
	}
};
// TO DO: Add more


class SinOperation final : public OperationExpressionElement
{
public:

	SinOperation() { type = OP_SIN; }
	~SinOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top(); calculationStack.pop(); //? что возвратит top/pop из пустого стека
		calculationStack.push(std::make_shared<ValueExpressionElement>(usin(*(rhs->value))));
	}
};




/*
* what's wrong
* 
class MultiplyOperation final : public OperationExpressionElement
{
public:

	MultiplyOperation() {}
	~MultiplyOperation() {}

	// A virtual method that runs operation's calculations based on the calculation stack, puts the result on top of the stack
	virtual void Calculate(std::stack<std::shared_ptr<ValueExpressionElement>>& calculationStack) override
	{
		auto rhs = calculationStack.top().get(); calculationStack.pop(); //? что возвратит top/pop из пустого стека
		auto lhs = (calculationStack.top()).get(); calculationStack.pop();

		calculationStack.push(std::make_shared<ValueExpressionElement>(*(rhs->value.get()) * *(lhs->value.get())));
		//? сложение принимает обычные -поинтеры- переменные, но возвращает shared
	}
};
*/