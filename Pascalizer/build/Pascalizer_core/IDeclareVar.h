#pragma once
#include "Instruction.h"

class IDeclareVar : public Instruction
{

public:

	IDeclareVar(Type type, std::string name) {}
	~IDeclareVar() override {}

	void Execute(ProgramState& programState) override {};
};