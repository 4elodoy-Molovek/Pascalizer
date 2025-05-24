#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>


/*
 * Program module, repsonsible for:
 *		1. Saving and Loading source code files;
 *		2. Modifying source code files;
 */
class FileModule
{
	// File path that was recently used to load a file
	std::string filePath;

	// Source code, loaded from a file
	std::string sourceCode;

public:

	FileModule() {}
	~FileModule() {}

	// Loads a source code file, saving it's contents into 'sourceCode'
	void LoadFile(const std::string& inFilePath)
	{
		std::ifstream inFile(inFilePath);

		if (!inFile.is_open()) throw(std::runtime_error("FILE MODULE: Failed to open file '" + inFilePath + "'!"));
		
		// Caching file path for later saving
		filePath = inFilePath;

		// Loading source code
		std::stringstream inCodeStream;

		std::string line;
		while (std::getline(inFile, line))
			inCodeStream << line << std::endl;

		sourceCode = inCodeStream.str();

		// Not forgetting to close the file
		inFile.close();
	}

	// Saves current source code to a new file
	void SaveFileAs(const std::string& inFilePath)
	{
		std::ofstream outFile(inFilePath);

		if (!outFile.is_open()) throw(std::runtime_error("FILE MODULE: Failed to write to file '" + inFilePath + "'!"));

		// Caching file path for later saving
		filePath = inFilePath;

		// Loading source code
		std::stringstream inCodeStream;

		outFile << sourceCode;

		// Not forgetting to close the file
		outFile.close();
	}

	// Saves current sourceCode to the recent file path;
	void SaveFile()
	{
		SaveFileAs(filePath);
	}

	// Returns current source code, allowing for it's modification
	std::string& GetSourceCode() { return sourceCode; }

};