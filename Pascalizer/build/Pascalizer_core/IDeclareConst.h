#pragma once
#include "Instruction.h"

class IDeclareConst : public Instruction
{

public:

	IDeclareConst(std::string constName, std::shared_ptr<Value> value) {}
	~IDeclareConst() override {}

	void Execute(ProgramState& programState) override {};
};