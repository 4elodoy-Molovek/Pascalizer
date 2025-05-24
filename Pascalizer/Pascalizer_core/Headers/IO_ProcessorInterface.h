#pragma once
#include <string>

class IO_ProcessorInterface
{
public:

	// Call for user input for I/O instructions
	virtual void CallReceiveUserInput(class IO_InstructionInterface* ioCaller) = 0;

	// Called when user input has been received
	virtual void OnUserInputReceived(const std::string& userInput) = 0;

	// Call for outputing data using IO interface
	virtual void CallOutputString(const std::string& output) = 0;
};