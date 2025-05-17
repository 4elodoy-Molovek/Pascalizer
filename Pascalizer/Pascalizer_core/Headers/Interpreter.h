#pragma once
#include <vector>
#include "Instruction.h"

/*
 * Program module, responsible for
 *		Executing analysized programs
 */

class Interpreter
{
	// Cached debug values
	std::vector<std::string> cachedLog;
	std::map<std::string, std::shared_ptr<Value>> cachedTable;

public:

	Interpreter();
	~Interpreter();

	// Executes the program
	void RunProgram(const HierarchicalList<Instruction*>& code)
	{
		ProgramState currentState(code);

		try
		{
			while (currentState.instructionPointer)
			{
				std::shared_ptr<Instruction> currentInstruction = currentState.instructionPointer->value;

				// Sets the next instruction pointer
				if (currentState.instructionPointer->pSub)
					currentState.instructionPointer = currentState.instructionPointer->pSub;
				else if (currentState.instructionPointer->pNext)
					currentState.instructionPointer = currentState.instructionPointer->pNext;
				else
					currentState.instructionPointer = currentState.instructionPointer->pUp->pNext;

				// Executes the instruction
				currentInstruction->Execute(currentState);

				// This way, if instruction modifies the intructionPointer, the program will work correctly
			}
		}

		catch (std::exception e)
		{
			// TO-DO:
			// Add error catching and cacheing
		}

		// Cacheing debug values
		cachedLog = currentState.log;
		cachedTable = currentState.valuesTable;
	}

	const std::vector<std::string>& GetCachedLog() { return cachedLog; }
	const std::map<std::string, std::shared_ptr<Value>>& GetCachedTable() { return cachedTable; }
};