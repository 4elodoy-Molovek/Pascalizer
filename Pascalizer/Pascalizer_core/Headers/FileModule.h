#pragma once
#include <string>


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

	FileModule();
	~FileModule();

	// Loads a source code file, saving it's contents into 'sourceCode'
	void LoadFile(const std::string& inFilePath);

	// Saves current sourceCode to the recent file path;
	void SaveFile();

	// Saves current source code to a new file
	void SaveFileAs(const std::string& inFilePath);

	// Returns current source code, allowing for it's modification
	std::string& GetSourceCode() { return sourceCode; }

};