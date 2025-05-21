#pragma once

#include "FileModule.h"
#include "Tokenizer.h"
#include "Interpreter.h"
#include "AnalysisMachine.h"


class Pascalizer
{
	// Modules
	FileModule fileModule;
	Tokenizer tokenizer;
	AnalysisMachine analysisMachine;
	Interpreter interpreter;

public:

	Pascalizer() {}
	~Pascalizer() {}

	FileModule& GetFileModule() { return fileModule; }

	// Gets source code from the last loaded file, analyses and runs it
	void InterpreteCurrentFileSourceCode();


	// Fetching debug info
	const std::vector<std::string>& GetCachedLog() { return interpreter.GetCachedLog(); }
	const ValuesTable& GetCachedValuesTable() { return interpreter.GetCachedTable(); }
};