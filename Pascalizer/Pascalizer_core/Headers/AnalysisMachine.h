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
	// Store index is interpreted by each accumulator in it's own way
	virtual void StoreElement(const Token& element, int storeIndex) = 0;

	// Creates an instruction out of stored data
	virtual Instruction* Collapse() = 0;
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
	void CheckTokenType(const Token& receivedToken, TokenType expectedTokenType)
	{
		if (receivedToken.type != expectedTokenType) return;
			//throw(std::exception("ANALYSIS ERROR: Unexpected token at token '" + receivedToken.value + "', expected:"));
	}

public:

	State(class AnalysisMachine& inMachine): parentMachine(inMachine) {}
	virtual ~State() {}

	// Called when the machine enters this state by traversing a connection based on the TokenizedElement 'element'
	virtual void EnterState(const Token& element) = 0;

	// Called when the machine exits this state
	virtual void ExitState() {}

	// Processes a single token
	// Additionally determines the next state of the machine based on the incoming element
	// If 'nullptr' is returned, that means that no transition is required
	virtual State* ProcessElement(const Token& nextElement) = 0;
};



/*
 * A state machine, that analyses tokenized source code step by step
 */
class AnalysisMachine
{
	// Vector of all states, filled up during the construction, used to destroy the machine
	std::vector<State*> states;

	// Resulting analyzed code
	HierarchicalList<std::shared_ptr<Instruction>> codeResult;
	
	// MACHINE STATE

	// Current state of the machine
	State* currentState;

	// Current accumulator of the machine
	Accumulator* currentAccumulator;


public:

	/*
	 * Where should the next instruction be added:
	 *		 0 - on the same level;
	 *		 1 - on the sub level;
	 *		-1 - on the parent level;
	 */
	int levelOffset = 0;

	// Used to handle single-line code blocks for branching
	int oneLinerDepth = 0;


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
		if (oneLinerDepth > 0)
		{
			codeResult.AddNextElement(instruction);
			oneLinerDepth--;

			return;
		}

			 if (levelOffset == 0)	codeResult.AddNextElement(instruction);
		else if (levelOffset == -1)	codeResult.AddUpElement(instruction);
		else if (levelOffset == 1)	codeResult.AddSubElement(instruction);

		else throw(std::exception("ANALYSIS DEVELOPMENT ERROR: Invalid level offset!"));
	
		levelOffset == 0;
	}

public:

	// Constructs the machine
	AnalysisMachine();
	~AnalysisMachine();

	// Processes a single source code element
	void ProcessElement(const Token& element)
	{
		State* newState = currentState->ProcessElement(element);
		if (newState)
		{
			currentState->ExitState();
			currentState = newState;
			newState->EnterState(element);
		}
	}

	// Returns the result of the analysis
	const HierarchicalList<std::shared_ptr<Instruction>>& GetResult() { return codeResult; }

	// Cleans up the state machine before the next use
	void CleanUp();

	//// State is made a friend class of the machine, to allow states to:
	////		- Create new Accumulators;
	////		- Store collapsed instructions into the resulting code.
	//friend class State;
};