#pragma once
#include "Instruction.h"

class IAssignVar : public Instruction
{

public:

	IAssignVar(std::string name, Expression expression) {}
	~IAssignVar() override {}

	void Execute(ProgramState& programState) override {};
};