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

	Interpreter() {}
	~Interpreter() {}

	// Executes the program
	void RunProgram(const HierarchicalList<std::shared_ptr<Instruction>>& code, class IO_ProcessorInterface* ioProcessor)
	{
		ProgramState currentState(code, ioProcessor);

		std::string errorCache = "";

		try
		{
			while (currentState.instructionPointer)
			{
				// Fetching the current instruction
				std::shared_ptr<Instruction> currentInstruction = currentState.instructionPointer->value;
				
				// Notifying the state of the currently executing instruction
				currentState.currentInstruction = currentState.instructionPointer;

				// Modifies instruction pointer, traversing the list in the top to bottom order (default)
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
			errorCache = "PROGRAM CRASH DETECTED: " + std::string(e.what());
		}

		// Cacheing debug values
		cachedLog = currentState.log;
		cachedLog.push_back(errorCache);

		cachedTable = currentState.valuesTable;
	}

	const std::vector<std::string>& GetCachedLog() { return cachedLog; }
	const std::map<std::string, std::shared_ptr<Value>>& GetCachedTable() { return cachedTable; }
};