#pragma once
#include <vector>
#include "Instruction.h"
#include "IO_ProcessorInterface.h"

/*
 * Program module, responsible for
 *		Executing analysized programs
 */

class Interpreter
{
	// Cached debug values
	std::vector<std::string> cachedLog;
	std::map<std::string, std::shared_ptr<Value>> cachedTable;

	ProgramState* currentState;

	void FinishProgram()
	{
		// Cacheing debug values
		cachedTable = currentState->valuesTable;
		currentState->ioProcessor->CallExecutionFinished();

		delete currentState;
	}

public:

	Interpreter() {}
	~Interpreter() {}

	// Executes the program
	void RunProgram(const HierarchicalList<std::shared_ptr<Instruction>>& code, class IO_ProcessorInterface* ioProcessor)
	{
		currentState = new ProgramState(code, ioProcessor);

		currentState->instructionPointer = currentState->code.GetFirst();

		ExecuteNextInstruction();

	}

	void ExecuteNextInstruction()
	{
		if (currentState)
		{
			if (!currentState->instructionPointer)
			{
				FinishProgram(); 
				return;
			}

			try
			{
				currentState->currentInstruction = currentState->instructionPointer;

				// Modifies instruction pointer, traversing the list in the top to bottom order (default)
				if (currentState->instructionPointer->pSub)
				{
					currentState->instructionPointer = currentState->instructionPointer->pSub;
					currentState->codeBlockExitInstructionPointerStack.push(currentState->currentInstruction->pNext);
				}

				else if (currentState->instructionPointer->pNext)
					currentState->instructionPointer = currentState->instructionPointer->pNext;

				else if (!currentState->codeBlockExitInstructionPointerStack.empty() && currentState->codeBlockExitInstructionPointerStack.top())
				{
					currentState->instructionPointer = currentState->codeBlockExitInstructionPointerStack.top();
					currentState->codeBlockExitInstructionPointerStack.pop();
				}

				else
				{
					currentState->instructionPointer = nullptr;
				}

				// Executes the instruction
				currentState->currentInstruction->value->Execute(*currentState);

				// This way, if instruction modifies the intructionPointer or codeBlockExitInstructionPointerStack, the program will work correctly

				// Updating cached log for runtime info in the UI
				cachedLog = currentState->log;

				// Continue execution if we dont have an IO block
				if (!currentState->ioBlock)
					ExecuteNextInstruction();
			}

			catch (std::exception e)
			{
				cachedLog.push_back("PROGRAM CRASH DETECTED: " + std::string(e.what()));
				FinishProgram();
			}
		}
	}

	void ProcessInstructionUserInput(IO_InstructionInterface* instruction, const std::string& input)
	{
		if (currentState)
		{
			try
			{
				instruction->OnUserInputReceived(input);

				if (!currentState->ioBlock)
					ExecuteNextInstruction();
			}

			catch (std::exception e)
			{
				cachedLog.push_back("PROGRAM CRASH DETECTED: " + std::string(e.what()));
				FinishProgram();
			}
		}
	}

	const std::vector<std::string>& GetCachedLog() 
	{
		if (currentState) return currentState->log;
		return cachedLog; 
	}
	const std::map<std::string, std::shared_ptr<Value>>& GetCachedTable()
	{
		//if (currentState) return currentState->valuesTable;
		return cachedTable;
	}
};