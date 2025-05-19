#pragma once
#include "AnalysisMachine.h"
#include "ExpressionEvaluationBlock.h"


/*
 * Collapses into std::shared_ptr<Expression>
 */
class ExpressionAccumulator : public Accumulator
{
	// Storage
	std::vector<std::shared_ptr<ExpressionElement>> postfix;
	std::stack<std::shared_ptr<OperationExpressionElement>> operationStack;

	std::string lastName;


public:
	ExpressionAccumulator() {}
	~ExpressionAccumulator() {}

	// Stores an element in the accumulator
	virtual void StoreElement(const Token& element, int storeIndex) override {}

	// Creates an instruction out of stored data
	virtual Instruction* Collapse() override
	{

	}

	// Adds a value to the postfix
	void AddValue(std::shared_ptr<Value> value) {}

	// Pushes operation into the operation stack
	void PushOperation(std::shared_ptr<OperationExpressionElement> operation) {}

	// Dumps the opration stack into postfix until it meets the break point
	void DumpOperationStack() {}
};


/*
 * Processes any expression expected in the code
 * Uses ExpressionAccumulator
 */
class ExpressionAnalysisBlockState : public State
{
public:

	// Expression analysis block has it's own custom accumulator, where it stores current expression data
	ExpressionAccumulator accumulator;
	
	// The state, to which analysis machine needs to go to after the analysis is finished
	State* analysisExitState;

public:
	ExpressionAnalysisBlockState(const AnalysisMachine& analysisMachine) : State(analysisMachine) {}


	~ExpressionAnalysisBlockState() {}

	// Called when the machine enters this state by traversing a connection based on the TokenizedElement 'element'
	virtual void EnterState(const Token& element) override
	{

	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override {}
};