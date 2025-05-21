#pragma once
#include "Instruction.h"

class IIf : public Instruction
{

public:

	IIf(Expression expression) {}
	~IIf() override {}

	void Execute(ProgramState& programState) override {};
};