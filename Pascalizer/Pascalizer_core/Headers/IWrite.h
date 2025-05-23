#pragma once
#include "Instruction.h"

class IWrite : public Instruction
{

public:

	IWrite(std::vector<std::shared_ptr<Value>> values) {}
	~IWrite() override {}

	void Execute(ProgramState& programState) override {};
};