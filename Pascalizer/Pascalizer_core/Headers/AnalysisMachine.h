#pragma once
#include <vector>
#include <stdexcept>
#include "Tokens.h"
#include "Instruction.h"
#include "AnalysisMachine_ExpressionAnalysisBlock.h"

/*
 * Program module, responsible for:
 *		1. Analysing tokenized source code;
 *		2. Locating syntax errors;
 */

/*
 * A base class for the machine's accumulation units
 */
class Accumulator
{
	// Child classes will contain data, accumulated while traversing the state machine

public:

	Accumulator() {}
	virtual ~Accumulator() {}

	// Stores an element in the accumulator
	// Store index is interpreted by each accumulator in it's own way
	virtual void StoreElement(const Token& element, int storeIndex) = 0;

	// Creates an instruction out of stored data
	virtual std::vector<std::shared_ptr<Instruction>> Collapse() = 0;
};



/*
 * A base class for the machine's states
 */
class State
{

protected:

	// Reference to the parent machine
	class AnalysisMachine& parentMachine;

	// Child classes will store pointers to outgoing states

protected:

	// If token type doesn'y match the expected one: throws an error: "Received token type doesn't match the expected one"
	void CheckTokenType(const Token& receivedToken, const std::set<TokenType>& expectedTokenTypes)
	{
		if (expectedTokenTypes.count(receivedToken.type) == 0)
			throw(std::runtime_error("ANALYSIS ERROR: Unexpected token at token '" + receivedToken.value + "'!"));
	}

public:

	State(class AnalysisMachine& inMachine): parentMachine(inMachine) {}
	virtual ~State() {}

	// Called when the machine enters this state by traversing a connection based on the TokenizedElement 'element'
	virtual void EnterState(const Token& element) = 0;

	// Called when the machine exits this state
	virtual void ExitState() {}

	/*
	 *	Processes a single token
	 *	Additionally determines the next state of the machine based on the incoming element
	 *	If 'nullptr' is returned, that means that no transition is required
	 *  If non-nullptr state is returned, then the machine transitions to the given state on the same cycle
	 *	entering the given state with the same token as was sent to this function
	 */
	virtual State* ProcessElement(const Token& nextElement) = 0;
};


enum AnalysisStatus { ONGOING, FINISHED, ERROR };

/*
 * A state machine, that analyses tokenized source code step by step
 */
class AnalysisMachine
{
	// Vector of all states, filled up during the construction, used to destroy the machine
	std::vector<State*> states;

	// A pointer to the initial state of the machine (used for resetting the state machine for continuos use)
	State* initialState;

	// Resulting analyzed code
	HierarchicalList<std::shared_ptr<Instruction>> codeResult;

	// MACHINE STATE

	// Current state of the machine
	State* currentState;

	// Current status of the analysis process
	AnalysisStatus analysisStatus = ONGOING;

	// Errors, that were encountered during the analysis process
	std::vector<std::string> analysisErrorLog;


	// EXPRESSION ANALYSIS BLOCK

	// Pointer to the state, that analyses expressions
	ExpressionAnalysisBlockState* expressionAnalysisBlock;

	// State the analysis machine is going to transition to when exiting the expression analysis block
	State* expressionBlockExitTarget;

	// A reference, where the result of expression analysis block's work will be put
	std::shared_ptr<Expression>* cachedExpressionAnalysisResultOutput;


public:
	// Current accumulator of the machine
	Accumulator* currentAccumulator;

	/*
	 * Where should the next instruction be added:
	 *		 0 - on the same level;
	 *		 1 - on the sub level;
	 *		-1 - on the parent level;
	 */
	int levelOffset = 0;

	// Used to handle single-line code blocks for branching
	int oneLinerDepth = 0;

	// A pointer to the instruction, that was most recently stored
	std::shared_ptr<Instruction> lastStoredInstruction;


	// Creates new a new accumulator, template because Accumulator is just an abstract base class
	template<class T>
	T* CreateAccumulator()
	{
		if (currentAccumulator) delete currentAccumulator;
		currentAccumulator = nullptr;

		T* newAccumulator = new T();
		currentAccumulator = T*;

		return newAccumulator;
	}

	// Stores the instruction into the result
	void StoreInstruction(std::shared_ptr<Instruction> instruction)
	{
		lastStoredInstruction = instruction;

		if (oneLinerDepth > 0)
		{
			codeResult.AddNextElement(instruction);
			oneLinerDepth--;

			return;
		}

				if (levelOffset == 0)	codeResult.AddNextElement(instruction);
		else	if (levelOffset == -1)	codeResult.AddUpElement(instruction);
		else	if (levelOffset == 1)	codeResult.AddSubElement(instruction);

		else throw(std::exception("ANALYSIS DEVELOPMENT ERROR: Invalid level offset!"));

		levelOffset == 0;
	}

	// Forcefully transitions the machine to a different state
	// Is used for imidiate state exits (like block exits)
	void ForceTransitionToState(State* state, const Token& enterElement)
	{
		currentState->ExitState();
		currentState = state;
		state->EnterState(enterElement);
	}

	// Called, when the state machine has reached the end of analysis
	void AnalysisFinished()
	{
		analysisStatus = FINISHED;
	}


	// EXPRESSION ANALYSIS BLOCK

	/*	Get the machine to enter the expression analysis state
	 *	Machine will stay in this state until it reaches END_LINE or COMMA or BRACKET_CLOSED (that doesn't close an expression open bracket)
	 *	Once the analysis has been finished, machine will enter @exitTarget state with the finishing token
	 *	The result of the analysis will be stored in @outResult
	 *  @entryToken is just passed to the EnterState of expression analysis block
	 */
	void EnterExpressionAnalysisState(State* exitTarget, std::shared_ptr<Expression>* outResult, const Token& entryToken)
	{
		expressionBlockExitTarget = exitTarget;
		cachedExpressionAnalysisResultOutput = outResult;

		// Transitioning to the analysis block, without calling exit on the current state (it is not finished yet)
		currentState = expressionAnalysisBlock;
		currentState->EnterState(entryToken);
	}

	// Called by the expression analysis state, once the analysis has been finished
	void ExpressionAnalysisFinished(std::shared_ptr<Expression> result, const Token& exitToken)
	{
		// Storing result in the requested location
		*cachedExpressionAnalysisResultOutput = result;

		// Returning to the state that called the analysis block
		currentState->ExitState();
		currentState = expressionBlockExitTarget;

		// No EnterState is required here as the state is already in process of analysing it's part
		// Instead, we are using process element to CONTINUE execution of the state
		ProcessElement(exitToken);
	}

public:

	// Constructs the machine
	AnalysisMachine();
	~AnalysisMachine();

	// Processes a single source code element
	void ProcessElement(const Token& element)
	{
		if (analysisStatus == ONGOING)
		{
			try
			{
				State* newState = currentState->ProcessElement(element);
				if (newState)
				{
					currentState->ExitState();
					currentState = newState;
					newState->EnterState(element);
				}
			}

			catch (std::exception e)
			{
				analysisErrorLog.push_back(std::string(e.what()));
				analysisStatus = ERROR;
			}
		}
	}

	// Returns the result of the analysis
	const HierarchicalList<std::shared_ptr<Instruction>>& GetResult() { return codeResult; }

	// Cleans up the state machine before the next use
	void CleanUp();

	AnalysisStatus GetStatus() { return analysisStatus; }

	const std::vector<std::string>& GetErrorLog() { return analysisErrorLog; }

	//// State is made a friend class of the machine, to allow states to:
	////		- Create new Accumulators;
	////		- Store collapsed instructions into the resulting code.
	//friend class State;
};