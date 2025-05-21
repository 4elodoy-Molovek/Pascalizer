#pragma once
#include "Instruction.h"

class IProgram : public Instruction
{

public:

	IProgram(std::string programName) {}
	~IProgram() override {}

	void Execute(ProgramState& programState) override {};
};