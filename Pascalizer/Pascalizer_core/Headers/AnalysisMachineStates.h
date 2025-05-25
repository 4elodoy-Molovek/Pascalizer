#pragma once
#include "AnalysisMachine.h"
#include "AnalysisMachineAccumulators.h"
#include "ProgramInstructions.h"


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
		innerState = 0;

		accumulator = parentMachine.CreateAccumulator<ProgramVerificationAccumulator>();
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	virtual State* ProcessElement(const Token& nextElement) override
	{
		// Processing "Program" keyword
		if (innerState == 0)
		{
			CheckTokenType(nextElement, { PROGRAM });

			innerState++;
			return nullptr;
		}

		// Processing progam name
		if (innerState == 1)
		{
			CheckTokenType(nextElement, { NAME });

			accumulator->StoreElement(nextElement, 0);

			innerState++;
			return nullptr;
		}

		// End line processig -> Collapsing the accumulator
		if (innerState == 2)
		{
			CheckTokenType(nextElement, { END_LINE });

			parentMachine.StoreInstruction(accumulator->Collapse()[0]);

			innerState++;
			return nullptr;
		}

		// Processing the following token -> Transition
		if (innerState == 3)
		{
			parentMachine.levelOffset++;

			CheckTokenType(nextElement, { CONST, VAR, BEGIN });

			if (nextElement.type == CONST)	return constBlockState;
			if (nextElement.type == VAR)	return varBlockState;
			if (nextElement.type == BEGIN)	return mainBlockBeginState;

			return nullptr;
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
		parentMachine.CreateAccumulator<GenericInstructionAccumulator<IConstBlock>>();
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	virtual State* ProcessElement(const Token& nextElement) override 
	{
		CheckTokenType(nextElement, { NAME });
		parentMachine.StoreInstruction(parentMachine.currentAccumulator->Collapse()[0]);

		parentMachine.levelOffset++;
		return constDeclarationState;
	}
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

	// Inner state
	int innerState = 0;

	ConstantDeclarationAccumulator* accumulator;

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
		innerState = 0;

		accumulator = parentMachine.CreateAccumulator<ConstantDeclarationAccumulator>();

		// We have entered the state with the name token, so we need to store it right away
		accumulator->StoreElement(element, 0);
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	virtual State* ProcessElement(const Token& nextElement) override
	{
		// state 0: name was processed in the EnterState
		if (innerState == 1)
		{
			CheckTokenType(nextElement, { COLON });

			innerState++;
			return nullptr;
		}

		// Type
		if (innerState == 1)
		{
			CheckTokenType(nextElement, { NAME });
			accumulator->StoreElement(nextElement, 1);

			innerState++;
			return nullptr;
		}

		// Assign symbol
		if (innerState == 2)
		{
			CheckTokenType(nextElement, { EQUAL });

			innerState++;
			return nullptr;
		}

		// Value
		if (innerState == 3)
		{
			CheckTokenType(nextElement, {VALUE_INT, VALUE_DOUBLE, VALUE_STRING});
			accumulator->StoreElement(nextElement, 2);

			innerState++;
			return nullptr;
		}

		// Endline + COLLAPSE
		if (innerState == 4)
		{
			CheckTokenType(nextElement, { END_LINE });
			parentMachine.StoreInstruction(accumulator->Collapse()[0]);

			innerState++;
			return nullptr;
		}

		// NEXT STATE
		if (innerState == 5)
		{
			CheckTokenType(nextElement, {NAME, VAR, BEGIN});

			if (nextElement.type == NAME) return this;
			if (nextElement.type == VAR) return exitToVarBlockState;
			if (nextElement.type == BEGIN) { parentMachine.levelOffset--; return mainBlockBeginState; }

			return nullptr;
		}
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

	Accumulator* accumulator;

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
		accumulator = parentMachine.CreateAccumulator<GenericInstructionAccumulator<IVarBlock>>();
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	virtual State* ProcessElement(const Token& nextElement) override 
	{
		CheckTokenType(nextElement, { NAME });

		std::vector<std::shared_ptr<Instruction>> varDeclarationInstructions = accumulator->Collapse();

		for (auto& instr: varDeclarationInstructions)
			parentMachine.StoreInstruction(instr);
		
		parentMachine.levelOffset++;
		return varDeclarationState;
	}
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

	// Inner state
	int innerState = 0;

	VariableDeclarationAccumulator* accumulator;

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
		innerState = 0;

		accumulator = parentMachine.CreateAccumulator<VariableDeclarationAccumulator>();

		// State is entered with a name token, storing it imidiately
		accumulator->StoreElement(element, 0);
		innerState++;
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override
	{
		if (innerState == 0)
		{
			CheckTokenType(nextElement, { NAME });
			accumulator->StoreElement(nextElement, 0);

			innerState++;
			return nullptr;
		}

		// Expecting a comma or a colon
		if (innerState == 1)
		{
			CheckTokenType(nextElement, { COMMA, COLON });

			// Reverting to state 0
			if (nextElement.type == COMMA)
			{
				innerState = 0;
				return nullptr;
			}
			
			// Continuing to type declaration
			innerState++;
			return nullptr;
		}

		// Type
		if (innerState == 2)
		{
			CheckTokenType(nextElement, { NAME });
			accumulator->StoreElement(nextElement, 1);

			innerState++;
			return nullptr;
		}

		// Endline + COLLAPSE
		if (innerState == 3)
		{
			CheckTokenType(nextElement, { END_LINE });
			
			parentMachine.StoreInstruction(accumulator->Collapse()[0]);
			
			innerState++;
			return nullptr;
		}

		// NEXT STATE
		if (innerState == 4)
		{
			CheckTokenType(nextElement, {NAME, CONST, BEGIN});

			if (nextElement.type == NAME) return this;
			if (nextElement.type == CONST) return exitToConstBlockState;
			if (nextElement.type == BEGIN) { parentMachine.levelOffset--; return mainBlockBeginState; }

			return nullptr;
		}
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
		parentMachine.levelOffset--;
		parentMachine.ForceTransitionToState(nextBlockState, element);
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override { return nullptr; }
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

	Accumulator* accumulator;

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
		accumulator = parentMachine.CreateAccumulator<GenericInstructionAccumulator<IMainBlock>>();
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override 
	{
		parentMachine.StoreInstruction(accumulator->Collapse()[0]);
		
		CheckTokenType(nextElement, {NAME, IF, END});

		if (nextElement.type == NAME) return nameState;
		if (nextElement.type == IF || nextElement.type == WHILE) return branchingState;
		if (nextElement.type == END) return endBlockState;

		return nullptr;
	}
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

	// Cache
	Token nameToken;

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
		// The state is entered with a name, caching it for the time being
		nameToken = element;
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override 
	{
		CheckTokenType(nextElement, { ASSIGN_OPERATOR, BRACKET_OPEN });

		// Creating accumulators and transitioning to the state based on the next token

		if (nextElement.type == ASSIGN_OPERATOR)
		{
			AssignVariableAccumulator* accumulator = parentMachine.CreateAccumulator<AssignVariableAccumulator>();
			accumulator->StoreElement(nameToken, 0);
			
			return variableAssignState;
		}

		if (nextElement.type == BRACKET_OPEN)
		{
			FunctionCallAccumulator* accumulator = parentMachine.CreateAccumulator<FunctionCallAccumulator>();
			accumulator->StoreElement(nameToken, 0);

			return functionCallState;
		}

		return nullptr;
	}
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

	// Inner state
	int innerState = 0;

	// Contains the result of the latest expression analysis, called by this state
	std::shared_ptr<Expression> expressionResult;

	AssignVariableAccumulator* accumulator;

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
		innerState = 0;

		// Fetching accumulator from the machine
		// Here we relly on the NameState to create a correct accumulator
		accumulator = dynamic_cast<AssignVariableAccumulator*>(parentMachine.currentAccumulator);

		// Entering expression analysis state right away
		parentMachine.EnterExpressionAnalysisState(this, &expressionResult, element);
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override 
	{
		// Variable name and equals sign were already aquared in the NameBlockState

		// CAME BACK FROM THE EXPRESSION BLOCK
		if (innerState == 0)
		{
			CheckTokenType(nextElement, { END_LINE });

			accumulator->StoreExpression(expressionResult, 1);

			parentMachine.StoreInstruction(accumulator->Collapse()[0]);

			innerState++;
			return nullptr;
		}

		// NEXT STATE
		if (innerState == 1)
		{
			CheckTokenType(nextElement, {NAME, IF, END});

			if (nextElement.type == NAME) return nameState;
			if (nextElement.type == IF || nextElement.type == WHILE) return branchingState;
			if (nextElement.type == END) return endBlockState;

			return nullptr;
		}
	}
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

	// Inner state
	int innerState = 0;

	// Contains the result of the latest expression analysis, called by this state
	std::shared_ptr<Expression> expressionResult;

	FunctionCallAccumulator* accumulator;

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
		innerState = 0;

		// Fetching accumulator from the machine
		// Here we relly on the NameState to create a correct accumulator
		accumulator = dynamic_cast<FunctionCallAccumulator*>(parentMachine.currentAccumulator);

		// Entering expression analysis right away
		parentMachine.EnterExpressionAnalysisState(this, &expressionResult, element);
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override 
	{
		// Function name and openning bracket were already aquared in the NameBlockState

		// CAME BACK FROM THE EXPRESSION BLOCK
		if (innerState == 0)
		{
			// Storing expression argument
			accumulator->StoreExpression(expressionResult, 1);

			CheckTokenType(nextElement, {COMMA, BRACKET_CLOSE});

			// Checking for more arguments
			if (nextElement.type == COMMA)
			{
				innerState = 0;
				parentMachine.EnterExpressionAnalysisState(this, &expressionResult, nextElement);

				return nullptr;
			}

			// Closign bracket case

			innerState++;
			return nullptr;
		}

		// End line + COLLAPSE
		if (innerState == 1)
		{
			CheckTokenType(nextElement, { END_LINE });

			parentMachine.StoreInstruction(accumulator->Collapse()[0]);

			innerState++;
			return nullptr;
		}

		// NEXT STATE
		if (innerState == 2)
		{
			CheckTokenType(nextElement, {NAME, IF, END});

			if (nextElement.type == NAME) return nameState;
			if (nextElement.type == IF || nextElement.type == WHILE) return branchingState;
			if (nextElement.type == END) return endBlockState;

			return nullptr;
		}
	}
};



// ===============
// Branching Block
// ===============

/*
 * Processes the beginning of branching operations like If and While
 * Acts as a rerouter, by just force transitioning the machine to either the IfState or the WhileState
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
		if (element.type == IF)
		{
			parentMachine.ForceTransitionToState(ifState, element);

			return;
		}

		if (element.type == WHILE)
		{
			parentMachine.ForceTransitionToState(whileState, element);

			return;
		}

		// Will never happen, but still
		CheckTokenType(element, { IF, WHILE });
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override { return nullptr; }
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

	// Inner State
	int innerState = 0;

	// Contains the result of the latest expression analysis, called by this state
	std::shared_ptr<Expression> expressionResult;

	IfAccumulator* accumulator;

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
		innerState = 0;

		accumulator = parentMachine.CreateAccumulator<IfAccumulator>();
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	virtual State* ProcessElement(const Token& nextElement) override 
	{
		// Condition start
		if (innerState == 0)
		{
			CheckTokenType(nextElement, { BRACKET_OPEN });

			innerState++;
			
			parentMachine.EnterExpressionAnalysisState(this, &expressionResult, nextElement);
			return nullptr;
		}

		// BACK FROM EXPRESSION ANALYSIS
		if (innerState == 1)
		{
			CheckTokenType(nextElement, { BRACKET_CLOSE });

			// Storing condition
			accumulator->StoreExpression(expressionResult, 0);

			parentMachine.StoreInstruction(accumulator->Collapse()[0]);

			innerState++;
			return nullptr;
		}

		// NEXT STATE
		if (innerState == 2)
		{
			CheckTokenType(nextElement, {IF, WHILE, NAME, BEGIN});

			if (nextElement.type == IF || nextElement.type == WHILE || nextElement.type == NAME) return oneLinerHandlerState;
			if (nextElement.type == BEGIN) return subBlockBeginState;

			return nullptr;
		}
	}
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

	// Inner State
	int innerState = 0;

	// Contains the result of the latest expression analysis, called by this state
	std::shared_ptr<Expression> expressionResult;

	WhileAccumulator* accumulator;

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
		innerState = 0;

		accumulator = parentMachine.CreateAccumulator<WhileAccumulator>();
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override 
	{
		// Condition start
		if (innerState == 0)
		{
			CheckTokenType(nextElement, { BRACKET_OPEN });

			innerState++;

			parentMachine.EnterExpressionAnalysisState(this, &expressionResult, nextElement);
		}

		// BACK FROM EXPRESSION ANALYSIS
		if (innerState == 1)
		{
			CheckTokenType(nextElement, { BRACKET_CLOSE });

			// Storing condition
			accumulator->StoreExpression(expressionResult, 0);

			parentMachine.StoreInstruction(accumulator->Collapse()[0]);

			innerState++;
			return nullptr;
		}

		// NEXT STATE
		if (innerState == 2)
		{
			CheckTokenType(nextElement, { IF, WHILE, NAME, BEGIN });

			if (nextElement.type == IF || nextElement.type == WHILE || nextElement.type == NAME) return oneLinerHandlerState;
			if (nextElement.type == BEGIN) return subBlockBeginState;

			return nullptr;
		}
	}
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

	Accumulator* accumulator;

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
		accumulator = parentMachine.CreateAccumulator<GenericInstructionAccumulator<IElse>>();
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override 
	{
		if (!dynamic_cast<IIf*>(parentMachine.lastStoredInstruction.get()))
			throw std::exception("ANALYSIS ERROR: Else with no corresponding 'IF' before it!");

		parentMachine.StoreInstruction(accumulator->Collapse()[0]);

		CheckTokenType(nextElement, { IF, WHILE, NAME, BEGIN });

		if (nextElement.type == IF || nextElement.type == WHILE || nextElement.type == NAME) return oneLinerHandlerState;
		if (nextElement.type == BEGIN) return subBlockBeginState;

		return nullptr;
	}
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
		parentMachine.oneLinerDepth++;

		CheckTokenType(element, { IF, WHILE, NAME });

		if (element.type == IF || element.type == WHILE)
		{
			parentMachine.ForceTransitionToState(branchingState, element);
			return;
		}

		if (element.type == NAME)
		{
			parentMachine.ForceTransitionToState(nameState, element);
			return;
		}
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override { return nullptr; }
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
		parentMachine.levelOffset++;

		CheckTokenType(element, { IF, WHILE, NAME });

		if (element.type == IF || element.type == WHILE)
		{
			parentMachine.ForceTransitionToState(branchingState, element);
			return;
		}

		if (element.type == NAME)
		{
			parentMachine.ForceTransitionToState(nameState, element);
			return;
		}
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override { return nullptr; }
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
		parentMachine.levelOffset--;
	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	// Determines the next state of the machine based on the incoming element
	// If no transition is possible it means that the machine has encountered a syntax error
	virtual State* ProcessElement(const Token& nextElement) override 
	{
		CheckTokenType(nextElement, { IF, WHILE, NAME, ELSE, PROGRAMM_END });

		if (nextElement.type == NAME) return nameState;
		if (nextElement.type == IF || nextElement.type == WHILE) return branchingState;
		if (nextElement.type == ELSE) return elseState;
		
		if (nextElement.type == PROGRAMM_END)
		{
			parentMachine.AnalysisFinished();

			return nullptr;
		}
	}
};