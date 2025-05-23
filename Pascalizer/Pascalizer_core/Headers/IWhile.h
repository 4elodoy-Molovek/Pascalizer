#pragma once
#include "Instruction.h"

class IWhile : public Instruction
{

public:

	IWhile(Expression expression) {}
	~IWhile() override {}

	void Execute(ProgramState& programState) override {};
};