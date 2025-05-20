#pragma once
#include "AnalysisMachine.h"
#include "ExpressionEvaluationBlock.h"


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
	virtual std::shared_ptr<Instruction> Collapse() override
	{
		
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
	virtual std::shared_ptr<Instruction> Collapse() override
	{
		// COLLAPSING
		I instruction = I();

		return std::make_shared<Instruction>(instruction);
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
	virtual std::shared_ptr<Instruction> Collapse() override
	{

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
	virtual std::shared_ptr<Instruction> Collapse() override
	{

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

	}

	// Creates an instruction out of stored data
	virtual std::shared_ptr<Instruction> Collapse() override
	{

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

	}

	// Creates an instruction out of stored data
	virtual std::shared_ptr<Instruction> Collapse() override
	{

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
	virtual void StoreElement(const Token& element, int storeIndex) override
	{

	}

	// Creates an instruction out of stored data
	virtual std::shared_ptr<Instruction> Collapse() override
	{

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
	virtual void StoreElement(const Token& element, int storeIndex) override
	{

	}

	// Creates an instruction out of stored data
	virtual std::shared_ptr<Instruction> Collapse() override
	{

	}
};