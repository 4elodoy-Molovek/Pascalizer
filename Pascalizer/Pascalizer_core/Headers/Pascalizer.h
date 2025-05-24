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
	IO_InstructionInterface* cachedIOCaller;

public:

	Pascalizer() {}
	~Pascalizer() {}

	FileModule& GetFileModule() { return fileModule; }

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

			return;
		}

		// Analysis suceeded case
		interpreter.RunProgram(analysisMachine.GetResult(), this);

		UpdateLogs(interpreter.GetCachedLog(), interpreter.GetCachedTable());
	}


	// 
	void AddError(const std::string& error)
	{

	}

	void ClearErrors()
	{

	}

	void UpdateLogs(const std::vector<std::string>& log, const std::map<std::string, std::shared_ptr<Value>>& valuesTable)
	{

	}


	// Fetching debug info
	const std::vector<std::string>& GetCachedLog() { return interpreter.GetCachedLog(); }
	const std::map<std::string, std::shared_ptr<Value>>& GetCachedValuesTable() { return interpreter.GetCachedTable(); }


	// INPUT / OUTPUT
	
	// Receiving user input for I/O instructions
	virtual void CallReceiveUserInput(IO_InstructionInterface* ioCaller) override
	{
		cachedIOCaller = ioCaller;

		// TO DO : Actual IO request
	}

	// Called when user input has been received
	virtual void OnUserInputReceived(const std::string& userInput) override
	{
		if (cachedIOCaller) cachedIOCaller->OnUserInputReceived(userInput);
	}

	// Call for outputing data using IO interface
	virtual void CallOutputString(const std::string& output) override
	{
		// TO DO : Actual IO request
	}
};