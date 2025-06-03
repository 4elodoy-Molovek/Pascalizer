#pragma once

#include "FileModule.h"
#include "Tokenizer.h"
#include "Interpreter.h"
#include "AnalysisMachine.h"
#include "IO_ProcessorInterface.h"


class Pascalizer : public IO_ProcessorInterface
{
	// Modules
	FileModule fileModule;
	Tokenizer tokenizer;
	AnalysisMachine analysisMachine;
	Interpreter interpreter;

	// A pointer to the instruction, that most recently called for user input
	IO_InstructionInterface* cachedIOCaller = nullptr;

	// UI Interface for IO functionality and other stuff
	IO_UI_Interface* userInterface = nullptr;

public:

	Pascalizer() {}
	~Pascalizer() {}

	FileModule& GetFileModule() { return fileModule; }

	void SetUserInterface(IO_UI_Interface* inUserInterface) { userInterface = inUserInterface; }

	// Gets source code from the last loaded file, analyses and runs it
	void InterpreteCurrentFileSourceCode()
	{
		// Calling clean up on everything
		analysisMachine.CleanUp();

		ClearErrors();

		// Tokenizing code
		std::vector<Token> tokenizedCode = tokenizer.TokenizeCode(fileModule.GetSourceCode());

		// Passing the tokenized code into the analysis machine
		for (auto& token : tokenizedCode)
			analysisMachine.ProcessElement(token);

		// Analysis failed case
		if (analysisMachine.GetStatus() == ERROR)
		{
			for (auto& error : analysisMachine.GetErrorLog())
				AddError(error);

			userInterface->OnFailedToLauch();
			return;
		}

		if (analysisMachine.GetStatus() == ONGOING)
		{
			AddError("ANALYSIS ERROR: No program end found!");
			userInterface->OnFailedToLauch();
			return;
		}

		userInterface->OnLauched();

		// Analysis suceeded case
		interpreter.RunProgram(analysisMachine.GetResult(), this);

		if (interpreter.GetCachedErrors().size() > 0)
		{
			for (auto error : interpreter.GetCachedErrors())
				AddError(error);

			userInterface->OnFailedToLauch();
		}

		userInterface->Update();
	}


	// 
	void AddError(const std::string& error)
	{
		userInterface->AddError(error);
	}

	void ClearErrors()
	{
		userInterface->ClearErrors();
	}



	// Fetching debug info
	const std::vector<std::string>& GetCachedLog() { return interpreter.GetCachedLog(); }
	const std::map<std::string, std::shared_ptr<Value>>& GetCachedValuesTable() { return interpreter.GetCachedTable(); }


	// INPUT / OUTPUT
	
	// Receiving user input for I/O instructions
	virtual void CallReceiveUserInput(IO_InstructionInterface* ioCaller) override
	{
		cachedIOCaller = ioCaller;

		userInterface->Update();

		userInterface->ReceiveUserInput();
	}

	// Called when user input has been received
	virtual void OnUserInputReceived(const std::string& userInput) override
	{
		if (cachedIOCaller)
		{
			IO_InstructionInterface* localIOCaller = cachedIOCaller;
			cachedIOCaller = nullptr;
			interpreter.ProcessInstructionUserInput(localIOCaller, userInput);
			
			if (interpreter.GetCachedErrors().size() > 0)
			{
				for (auto error : interpreter.GetCachedErrors())
					AddError(error);

				userInterface->OnFailedToLauch();
			}
		}
	}

	// Call for outputing data using IO interface
	virtual void CallOutputString(const std::string& output) override
	{
		userInterface->Update();
		userInterface->OutputString(output);
	}

	virtual void CallExecutionFinished() override
	{
		userInterface->Update();
	}
};