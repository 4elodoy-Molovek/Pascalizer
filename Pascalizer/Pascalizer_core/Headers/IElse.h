#pragma once
#include "Instruction.h"

class IElse : public Instruction
{

public:

	IElse() {}
	~IElse() override {}

	void Execute(ProgramState& programState) override {};
};