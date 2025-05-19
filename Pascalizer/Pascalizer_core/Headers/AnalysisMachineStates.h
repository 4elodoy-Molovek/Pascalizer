#pragma once
#include "AnalysisMachine.h"
#include "AnalysisMachineAccumulators.h"


// ====================
// Program Verification
// ====================

/*
 * Processes the first line of the source code (Expecting "Program <name>;")
 * Uses ProgramVerificationAccumulator 
 */
class ProgramVerificationState : public State
{

protected:

	// Inner state of this analysis state
	int innerState = 0;

	// Pointer to the accumulator (Created and destoyed by the analysis machine)
	ProgramVerificationAccumulator* accumulator;

public:

	// Outgoing connections
	State* mainBlockBeginState;
	State* constBlockState;
	State* varBlockState;


public:
	ProgramVerificationState(AnalysisMachine& analysisMachine,
							 State* inMainBlockBeginState,
							 State* inConstBlockState,
							 State* inVarBlockState) 
							 : State(analysisMachine) 
	{
		mainBlockBeginState = inMainBlockBeginState;
		constBlockState = inConstBlockState;
		varBlockState = inVarBlockState;
	}


	~ProgramVerificationState() {}

	// Called when the machine enters this state by traversing a connection based on the TokenizedElement 'element'
	virtual void EnterState(const Token& element) override
	{
		parentMachine.CreateAccumulator<ProgramVerificationAccumulator>();
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Processes a single token
	// Additionally determines the next state of the machine based on the incoming element
	// If 'nullptr' is returned, that means that no transition is required
	virtual State* ProcessElement(const Token& nextElement) override
	{
		// Processing "Program" keyword
		if (innerState == 0)
		{
			CheckTokenType(nextElement, PROGRAM);

			innerState++;
			return nullptr;
		}

		// Processing progam name
		if (innerState == 1)
		{
			CheckTokenType(nextElement, NAME);

			accumulator->StoreElement(nextElement, 0);

			innerState++;
			return nullptr;
		}

		// End line processig -> Collapsing the accumulator
		if (innerState == 2)
		{
			CheckTokenType(nextElement, END_LINE);

			

			innerState++;
			return nullptr;
		}

		// Processing the following token -> Transition
		if (innerState == 3)
		{
			if (nextElement.type == CONST)	return constBlockState;
			if (nextElement.type == VAR)	return varBlockState;
			if (nextElement.type == BEGIN)	return mainBlockBeginState;

			CheckTokenType(nextElement, BEGIN);
		}

	}
};



// ===========
// Const Block
// ===========

/*
 * Processes constant block beginning (Expecting "const")
 * Uses GenericInstructionAccumulator<ConstBlockInstruction>
 * Enters sub-level
 */
class ConstBlockState : public State
{
public:

	// Outgoing connections
	State* constDeclarationState;


public:
	ConstBlockState(AnalysisMachine& analysisMachine,
					State* inConstDeclarationState) 
					: State(analysisMachine)
	{
		constDeclarationState = inConstDeclarationState;
	}


	~ConstBlockState() {}

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


/*
 * Processes constant declaration line (Expecting "name type = value")
 * Uses ConstantDeclarationAccumulator
 */
class ConstDeclarationState : public State
{
public:

	// Outgoing connections
	State* mainBlockBeginState;
	State* exitToVarBlockState;


public:
	ConstDeclarationState(AnalysisMachine& analysisMachine,
						  State* inMainBlockBeginState,
						  State* inExitToVarBlockState)
						  : State(analysisMachine)
	{
		mainBlockBeginState = inMainBlockBeginState;
		exitToVarBlockState = inExitToVarBlockState;
	}


	~ConstDeclarationState() {}

	// Called when the machine enters this state by traversing a connection based on the TokenizedElement 'element'
	virtual void EnterState(const Token& element) override
	{

	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override
	{

	}
};



// =========
// Var Block
// =========

/*
 * Processes variable block beginning (Expecting "var")
 * Uses GenericInstructionAccumulator<VarBlockInstruction>
 * Enters sub-level
 */
class VarBlockState : public State
{
public:

	// Outgoing connections
	State* varDeclarationState;


public:
	VarBlockState(AnalysisMachine& analysisMachine,
		State* inVarDeclarationState)
		: State(analysisMachine)
	{
		varDeclarationState = inVarDeclarationState;
	}


	~VarBlockState() {}

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


/*
 * Processes variable declaration line (Expecting "name, name type;")
 * Uses VariableDeclrationAccumulator
 */
class VarDeclarationState : public State
{
public:

	// Outgoing connections
	State* mainBlockBeginState;
	State* exitToConstBlockState;


public:
	VarDeclarationState(AnalysisMachine& analysisMachine,
		State* inMainBlockBeginState,
		State* inExitToConstBlockState)
		: State(analysisMachine)
	{
		mainBlockBeginState = inMainBlockBeginState;
		exitToConstBlockState = inExitToConstBlockState;
	}


	~VarDeclarationState() {}

	// Called when the machine enters this state by traversing a connection based on the TokenizedElement 'element'
	virtual void EnterState(const Token& element) override
	{

	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override
	{

	}
};



// ==========
// Block Exit
// ==========

/*
 * Processes the transition between const and var blocks
 * No accumulator needed
 * Enters parent-level
 */
class BlockExitState : public State
{
public:

	// Outgoing connections
	State* nextBlockState;


public:
	BlockExitState(AnalysisMachine& analysisMachine,
		State* inNextBlockState)
		: State(analysisMachine)
	{
		nextBlockState = inNextBlockState;
	}


	~BlockExitState() {}

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




// ================
// Main Block Begin
// ================

/*
 * Processes the beginning of the main code block
 * Uses GenericInstructionAccumulator<MainBlockInstruction>
 * Enters sub-level
 */
class MainBlockBeginState : public State
{
public:

	// Outgoing connections
	State* nameState;
	State* branchingState;
	State* endBlockState;


public:
	MainBlockBeginState(AnalysisMachine& analysisMachine,
		State* inNameState,
		State* inBranchingState,
		State* inEndBlockState)
		: State(analysisMachine)
	{
		nameState = inNameState;
		branchingState = inBranchingState;
		endBlockState = inEndBlockState;
	}


	~MainBlockBeginState() {}

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



// ==========
// Name Block
// ==========

/*
 * Processes the beginning of variable assignment and function call lines
 * Creates accumulators for the next states:
 *		- AssignVariableAccumulator;
 *		- FunctionCallAccumulator; 
 */
class NameBlockState : public State
{
public:

	// Outgoing connections
	State* variableAssignState;
	State* functionCallState;


public:
	NameBlockState(AnalysisMachine& analysisMachine,
		State* inVariableAssignState,
		State* inFunctionCallState)
		: State(analysisMachine)
	{
		variableAssignState = inVariableAssignState;
		functionCallState = inFunctionCallState;
	}


	~NameBlockState() {}

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


/*
 * Processing variable assigning operations (Expecting "name = <expression>;")
 * Uses expression analysis block
 * Uses AssignVariableAccumulator
 * One-liner support
 */
class VariableAssignState : public State
{
public:

	// Outgoing connections
	State* nameState;
	State* branchingState;
	State* endBlockState;


public:
	VariableAssignState(AnalysisMachine& analysisMachine,
		State* inNameState,
		State* inBranchingState,
		State* inEndBlockState)
		: State(analysisMachine)
	{
		nameState = inNameState;
		branchingState = inBranchingState;
		endBlockState = inEndBlockState;
	}


	~VariableAssignState() {}

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


/*
 * Processing function call operations (Expecting "name(<expression>, <expression>, ...);")
 * Uses expression analysis block
 * Uses FunctionCallAccumulator
 * One-liner support
 */
class FunctionCallState : public State
{
public:

	// Outgoing connections
	State* nameState;
	State* branchingState;
	State* endBlockState;


public:
	FunctionCallState(AnalysisMachine& analysisMachine,
		State* inNameState,
		State* inBranchingState,
		State* inEndBlockState)
		: State(analysisMachine)
	{
		nameState = inNameState;
		branchingState = inBranchingState;
		endBlockState = inEndBlockState;
	}


	~FunctionCallState() {}

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



// ===============
// Branching Block
// ===============

/*
 * Processes the beginning of branching operations like If and While
 * Creates accumulators for the next states:
 *		- IfAccumulator;
 *		- WhileAccumulator;
 */
class BranchingBlock : public State
{
public:

	// Outgoing connections
	State* ifState;
	State* whileState;


public:
	BranchingBlock(AnalysisMachine& analysisMachine,
		State* inIfState,
		State* inWhileState)
		: State(analysisMachine)
	{
		ifState = inIfState;
		whileState = inWhileState;
	}


	~BranchingBlock() {}

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


/*
 * Processes If branching (Expecting "If(<expression>) then")
 * Uses IfAccumulator
 */
class IfState : public State
{
public:

	// Outgoing connections
	State* oneLinerHandlerState;
	State* subBlockBeginState;

public:
	IfState(AnalysisMachine& analysisMachine,
		State* inOneLinerHandlerState,
		State* inSubBlockBeginState)
		: State(analysisMachine)
	{
		oneLinerHandlerState = inOneLinerHandlerState;
		subBlockBeginState = inSubBlockBeginState;
	}


	~IfState() {}

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


/*
 * Processes While branching (Expecting "While(<expression>) then")
 * Uses WhileAccumulator
 */
class WhileState : public State
{
public:

	// Outgoing connections
	State* oneLinerHandlerState;
	State* subBlockBeginState;

public:
	WhileState(AnalysisMachine& analysisMachine,
		State* inOneLinerHandlerState,
		State* inSubBlockBeginState)
		: State(analysisMachine)
	{
		oneLinerHandlerState = inOneLinerHandlerState;
		subBlockBeginState = inSubBlockBeginState;
	}


	~WhileState() {}

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


/*
 * Processes Else branching (Expecting "Else")
 * Only if last added instruction on this level was 'If'
 * Uses GenericInstructionAccumulator<Else>
 */
class ElseState : public State
{
public:

	// Outgoing connections
	State* oneLinerHandlerState;
	State* subBlockBeginState;

public:
	ElseState(AnalysisMachine& analysisMachine,
		State* inOneLinerHandlerState,
		State* inSubBlockBeginState)
		: State(analysisMachine)
	{
		oneLinerHandlerState = inOneLinerHandlerState;
		subBlockBeginState = inSubBlockBeginState;
	}


	~ElseState() {}

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


/*
 * Handles one line code blocks for branching
 * No need for accumulator
 * "one-liner++"
 */
class OneLinerHandlerState : public State
{
public:

	// Outgoing connections
	State* nameState;
	State* branchingState;

public:
	OneLinerHandlerState(AnalysisMachine& analysisMachine,
		State* inNameState,
		State* inBranchingState)
		: State(analysisMachine)
	{
		nameState = inNameState;
		branchingState = inBranchingState;
	}


	~OneLinerHandlerState() {}

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


/*
 * Processes the beginning of a new code block for branching
 * No need for accumulator
 * Enters sub-level
 */
class SubBlockBeginState : public State
{
public:

	// Outgoing connections
	State* nameState;
	State* branchingState;

public:
	SubBlockBeginState(AnalysisMachine& analysisMachine,
		State* inNameState,
		State* inBranchingState)
		: State(analysisMachine)
	{
		nameState = inNameState;
		branchingState = inBranchingState;
	}


	~SubBlockBeginState() {}

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



// =========
// End Block
// =========

/*
 * Processes the end of a code block and the end of the main code block (Expecting "end" or "end.")
 * No need for accumulator
 * Enters parent-level
 */
class EndBlockState : public State
{
public:

	// Outgoing connections
	State* nameState;
	State* branchingState;
	State* elseState;

public:
	EndBlockState(AnalysisMachine& analysisMachine,
		State* inNameState,
		State* inBranchingState,
		State* inElseState)
		: State(analysisMachine)
	{
		nameState = inNameState;
		branchingState = inBranchingState;
		elseState = inElseState;
	}


	~EndBlockState() {}

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