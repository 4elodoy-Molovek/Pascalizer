#pragma once
#include "AnalysisMachine.h"
#include "ExpressionEvaluationBlock.h"
#include "ProgramInstructions.h"
#include <stdexcept>


/*
 * Collapses into Program(string programName) instruction
 */
class ProgramVerificationAccumulator : public Accumulator
{
	// Storage
	std::string programName;

public:
	ProgramVerificationAccumulator() {}
	~ProgramVerificationAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override
	{
		programName = element.value;
	}

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() override
	{
		return { std::make_shared<IProgram>(programName) };
	}
};


/*
 * Templated accumulator that collapses into any instruction as long as it doesn't have
 * necessary construction parameters
 */
template <class I>
class GenericInstructionAccumulator : public Accumulator
{

public:
	GenericInstructionAccumulator() {}
	~GenericInstructionAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override {}

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() override
	{
		// COLLAPSING
		return { std::make_shared<I>() };
	}
};


/*
 * Collapses into DeclareConst(string name, Value* value) instruction
 */
class ConstantDeclarationAccumulator : public Accumulator
{
	// Storage
	std::string constName;
	std::string type; // TO DO: Replace with some Type enum
	std::string value;

public:
	ConstantDeclarationAccumulator() {}
	~ConstantDeclarationAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override
	{
		switch (storeIndex)
		{

		case 0: constName	= element.value; return;
		case 1: type		= element.value; return;
		case 2: value		= element.value; return;

		default: throw(std::exception("ANALYSIS DEVELOPMENT ERROR: Invalid store index passed into ConstantDeclarationAccumulator"));
		}
	}

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() override
	{

		if (type == "int")		return { std::make_shared<IDeclareConst>(constName, std::make_shared<IntValue>(std::stoi(value))) };
		if (type == "double")	return { std::make_shared<IDeclareConst>(constName, std::make_shared<DoubleValue>(std::stod(value))) };
		if (type == "string")	return { std::make_shared<IDeclareConst>(constName, std::make_shared<StringValue>(value)) };

		throw(std::runtime_error("ANALYSIS ERROR: Unidentified type '" + type + "'!"));
	}
};


/*
 * Collapses into multiple DeclareVar(string name, Value* value) instructions
 */
class VariableDeclarationAccumulator : public Accumulator
{
	// Storage
	std::vector<std::string> varNames;
	std::string type; // TO DO: Replace with some Type enum

public:
	VariableDeclarationAccumulator() {}
	~VariableDeclarationAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override
	{
		switch (storeIndex)
		{

		case 0: varNames.push_back(element.value); return;
		case 1: type = element.value; return;

		default: throw(std::exception("ANALYSIS DEVELOPMENT ERROR: Invalid store index passed into VariableDeclarationAccumulator"));
		}
	}

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() override
	{
		Type eType;

				if (type == "int") eType = INT;
		else	if (type == "double") eType = DOUBLE;
		else	if (type == "string") eType = STRING;
		else	throw(std::runtime_error("ANALYSIS ERROR: Unidentified type '" + type + "'!"));

		std::vector<std::shared_ptr<Instruction>> instructions;

		for (auto& name : varNames)
			instructions.push_back(std::make_shared<IDeclareVar>(eType, name));

		return instructions;
	}
};


/*
 * Collapses into AssignVariable(string name, Expression newValue)
 * Fetches expression block cached result
 */
class AssignVariableAccumulator : public Accumulator
{
	// Storage
	std::string varName;
	std::shared_ptr<Expression> newValueExpression;

public:
	AssignVariableAccumulator() {}
	~AssignVariableAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override
	{
		if (storeIndex == 0) varName = element.value;
	}

	// Stores an expression in the accumulator
	void StoreExpression(std::shared_ptr<Expression> expression, int storeIndex)
	{
		if (storeIndex == 1) newValueExpression = expression;
	}

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() override
	{
		return { std::make_shared<IAssignVar>(varName, newValueExpression) };
	}
};


/*
 * Collapses into any valid instruction
 * Fetches expression block cached result
 */
class FunctionCallAccumulator : public Accumulator
{
	// Storage
	std::string functionName;
	std::vector<std::shared_ptr<Expression>> argumentExpressions;

public:
	FunctionCallAccumulator() {}
	~FunctionCallAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override
	{
		if (storeIndex == 0) functionName = element.value;
	}

	// Stores an expression in the accumulator
	void StoreExpression(std::shared_ptr<Expression> expression, int storeIndex)
	{
		if (storeIndex == 1) argumentExpressions.push_back(expression);
	}

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() override
	{
		// Write function handling
		if (functionName == "Write")
		{
			return { std::make_shared<IWrite>(argumentExpressions) };
		}

		// Read function handling
		if (functionName == "Read")
		{
			return { std::make_shared<IRead>(argumentExpressions[0])};
		}

		throw(std::runtime_error("ANALYSIS ERROR: Unknown function '" + functionName + "'!"));
	}
};


/*
 * Collapses into If(Expression condition) instruction
 * Fetches expression block cached result
 */
class IfAccumulator : public Accumulator
{
	// Storage
	std::shared_ptr<Expression> conditionExpression;

public:
	IfAccumulator() {}
	~IfAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override {}

	// Stores an expression in the accumulator
	void StoreExpression(std::shared_ptr<Expression> expression, int storeIndex)
	{
		if (storeIndex == 0) conditionExpression = expression;
	}

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() override
	{
		return { std::make_shared<IIf>(conditionExpression) };
	}
};


/*
 * Collapses into While(Expression condition) instruction
 * Fetches expression block cached result
 */
class WhileAccumulator : public Accumulator
{
	// Storage
	std::shared_ptr<Expression> conditionExpression;

public:
	WhileAccumulator() {}
	~WhileAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override {}

	// Stores an expression in the accumulator
	void StoreExpression(std::shared_ptr<Expression> expression, int storeIndex)
	{
		if (storeIndex == 0) conditionExpression = expression;
	}

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() override
	{
		return { std::make_shared<IWhile>(conditionExpression) };
	}
};