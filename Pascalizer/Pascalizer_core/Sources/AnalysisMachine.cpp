#include "AnalysisMachine.h"
#include "AnalysisMachineStates.h"
#include "AnalysisMachine_ExpressionAnalysisBlock.h"



// Constructs the machine
AnalysisMachine::AnalysisMachine()
{
	/*
	 * Constructing the machine in the reverse order to make it easier to link states together,
	 * Since most of the linked states would already exist at the momement of state construction
	 * The only links we will be setting separately are the ones that form loops in the state machine graph
	 * 
	 * The picture respresentation of the state machine can be found in the repository, use it if you want
	 * to figure out / expand the machine
	 */


	// Initial backwards pass of construction ('nullptr'-s are going to be filled in the loop section)

	EndBlockState* endBlockState = new EndBlockState(*this, nullptr, nullptr, nullptr);

	SubBlockBeginState* subBlockBeginState = new SubBlockBeginState(*this, nullptr, nullptr);
	OneLinerHandlerState* oneLinerHandlerState = new OneLinerHandlerState(*this, nullptr, nullptr);

	IfState* ifState = new IfState(*this, oneLinerHandlerState, subBlockBeginState);
	WhileState* whileState = new WhileState(*this, oneLinerHandlerState, subBlockBeginState);
	ElseState* elseState = new ElseState(*this, oneLinerHandlerState, subBlockBeginState);
	BranchingBlock* branchingBlockState = new BranchingBlock(*this, ifState, whileState);

	VariableAssignState* variableAssignState = new VariableAssignState(*this, nullptr, branchingBlockState, endBlockState, elseState);
	FunctionCallState* functionCallState = new FunctionCallState(*this, nullptr, branchingBlockState, endBlockState, elseState);
	NameBlockState* nameBlockState = new NameBlockState(*this, variableAssignState, functionCallState);

	MainBlockBeginState* mainBlockBeginState = new MainBlockBeginState(*this, nameBlockState, branchingBlockState, endBlockState);

	ConstDeclarationState* constDeclarationState = new ConstDeclarationState(*this, mainBlockBeginState, nullptr);
	VarDeclarationState* varDeclarationState = new VarDeclarationState(*this, mainBlockBeginState, nullptr);
	ConstBlockState* cosntBlockState = new ConstBlockState(*this, constDeclarationState);
	VarBlockState* varBlockState = new VarBlockState(*this, varDeclarationState);
	BlockExitState* exitToVarBlockState = new BlockExitState(*this, varBlockState);
	BlockExitState* exitToConstBlockState = new BlockExitState(*this, constDeclarationState);

	ProgramVerificationState* programVerificationState = new ProgramVerificationState(*this, mainBlockBeginState, cosntBlockState, varBlockState);


	// Forming loops

	// endBLockState
	endBlockState->branchingState = branchingBlockState;
	endBlockState->elseState = elseState;
	endBlockState->nameState = nameBlockState;

	// subBlockBeginState
	subBlockBeginState->branchingState = branchingBlockState;
	subBlockBeginState->nameState = nameBlockState;

	// oneLinerHandlerState
	oneLinerHandlerState->branchingState = branchingBlockState;
	oneLinerHandlerState->nameState = nameBlockState;

	// variableAssignState
	variableAssignState->nameState = nameBlockState;

	// functionCallState
	functionCallState->nameState = nameBlockState;

	// constDeclarationState
	constDeclarationState->exitToVarBlockState = exitToVarBlockState;

	// varDeclarationState
	varDeclarationState->exitToConstBlockState = exitToConstBlockState;


	// Saving states to the state buffer (to delete on exit)
	states.push_back(endBlockState);
	states.push_back(subBlockBeginState);
	states.push_back(oneLinerHandlerState);
	states.push_back(ifState);
	states.push_back(elseState);
	states.push_back(branchingBlockState);
	states.push_back(variableAssignState);
	states.push_back(functionCallState);
	states.push_back(nameBlockState);
	states.push_back(mainBlockBeginState);
	states.push_back(constDeclarationState);
	states.push_back(varDeclarationState);
	states.push_back(cosntBlockState);
	states.push_back(varBlockState);
	states.push_back(exitToVarBlockState);
	states.push_back(exitToConstBlockState);
	states.push_back(programVerificationState);


	// Setting initial state
	initialState = programVerificationState;
	currentState = initialState;
	currentState->EnterState({});


	// Expression analysis block
	expressionAnalysisBlock = new ExpressionAnalysisBlockState(*this);
	states.push_back(expressionAnalysisBlock);
}

// Destroying the state machine
AnalysisMachine::~AnalysisMachine()
{
	for (auto& state : states)
		delete state;
}


void AnalysisMachine::EnterExpressionAnalysisState(State* exitTarget, std::shared_ptr<Expression>* outResult, const Token& entryToken)
{
	expressionBlockExitTarget = exitTarget;
	cachedExpressionAnalysisResultOutput = outResult;

	// Transitioning to the analysis block, without calling exit on the current state (it is not finished yet)
	currentState = expressionAnalysisBlock;
	currentState->EnterState(entryToken);
}

void AnalysisMachine::ExpressionAnalysisFinished(std::shared_ptr<Expression> result, const Token& exitToken)
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


void AnalysisMachine::ProcessElement(const Token& element)
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

void AnalysisMachine::CleanUp()
{
	codeResult.Clear();
	currentState = initialState;
	currentState->EnterState({});

	levelOffset = 0;
	oneLinerDepth = 0;

	analysisErrorLog.clear();
	analysisStatus = ONGOING;
}
