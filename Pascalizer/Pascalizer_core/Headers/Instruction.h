#pragma once
#include "ProgramState.h"


/*
 * A base class for program instructions, that are generated in Analysis Machine and later executed in the interpreter
 */
class Instruction
{

public:

	Instruction() {}
	virtual ~Instruction() {}

	// Executes the instruction
	// NOTE: instructions may modify program state by adding new values to the table or changing the Instruction Pointer
	virtual void Execute(ProgramState& programState) = 0;

	// Used for testing
	// Retunns "Instruction(arguments)"
	virtual std::string GetStringNotation() = 0;
};


// Used by instructions that receive user input
class IO_InstructionInterface
{

public:

	// Called when the user input has been received after this instruction requested it
	virtual void OnUserInputReceived(const std::string& userInput) = 0;
};