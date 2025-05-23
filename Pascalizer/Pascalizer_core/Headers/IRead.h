#pragma once
#include "Instruction.h"

class IRead : public Instruction
{

public:

	IRead(std::string varName) {}
	~IRead() override {}

	void Execute(ProgramState& programState) override {};
};