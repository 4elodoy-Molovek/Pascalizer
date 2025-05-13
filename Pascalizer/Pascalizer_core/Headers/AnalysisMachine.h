#pragma once
#include <vector>
#include "Tokens.h"
#include "Instruction.h"

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
	virtual void StoreElement(const TokenizedElement& element) = 0;

	// Creates an instruction out of stored data
	virtual Instruction* Collapse() = 0;
};



/*
 * A base class for the machine's states
 */
class State
{
	// Reference to the parent machine
	const class AnalysisMachine& parentMachine;

	// Child classes will store pointers to outgoing states

public:

	State(const class AnalysisMachine& inMachine): parentMachine(inMachine) {}
	virtual ~State() {}

	// Called when the machine enters this state by traversing a connection based on the TokenizedElement 'element'
	virtual void EnterState(const TokenizedElement& element) = 0;

	// Called when the machine exits this state
	virtual void ExitState() {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* UpdateTransitions(const TokenizedElement& nextElement) = 0;
};



/*
 * A state machine, that analyses tokenized source code step by step
 */
class AnalysisMachine
{
	// Vector of all states, filled up during the construction, used to destroy the machine
	std::vector<State*> states;

	// Resulting analyzed code
	HierarchicalList<Instruction*> codeResult;

	// Current state of the machine
	State* currentState;

	// Current accumulator of the machine
	Accumulator* currentAccumulator;


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

public:

	// Constructs the machine
	AnalysisMachine();
	~AnalysisMachine();

	// Processes a single source code element
	void ProcessElement(const TokenizedElement& element)
	{
		State* newState = currentState->UpdateTransitions(element);
		if (newState)
		{
			currentState->ExitState();
			currentState = newState;
			newState->EnterState(element);
		}
	}

	// Returns the result of the analysis
	const HierarchicalList<Instruction*>& GetResult() { return codeResult; }

	// Cleans up the state machine before the next use
	void CleanUp();

	// State is made a friend class of the machine, to allow states to:
	//		- Create new Accumulators;
	//		- Store collapsed instructions into the resulting code.
	friend class State;
};