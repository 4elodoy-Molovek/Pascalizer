#pragma once
#include "Instruction.h"
#include "ExpressionEvaluationBlock.h"


// Head of the program
class IProgram : public Instruction
{
private:
	std::string _programName;

public:

	IProgram(const std::string& programName) 
	{
		_programName = programName;
	}
	~IProgram() override {}

	void Execute(ProgramState& programState) override 
	{
		programState.log.push_back("Program started, name: " + _programName);
	};
};

// 'const' keyword, logs the start of the const section
class IConstBlock : public Instruction
{

public:

	IConstBlock() {}
	~IConstBlock() override {}

	void Execute(ProgramState& programState) override 
	{
		programState.log.push_back("Const block started");
	};
};

// 'var' keyword, logs the start of the var section
class IVarBlock : public Instruction
{

public:

	IVarBlock() {}
	~IVarBlock() override {}

	void Execute(ProgramState& programState) override 
	{
		programState.log.push_back("Var block started");
	};
};

// 'begin' keyword of the main code block, logs the start of the main code block section
class IMainBlock : public Instruction
{

public:

	IMainBlock() {}
	~IMainBlock() override {}

	void Execute(ProgramState& programState) override 
	{
		programState.log.push_back("Main block started");
	};
};



// Registers a new constant value in the value table and assigns it a default value
class IDeclareConst : public Instruction
{
private:
	std::string _constName;
	std::shared_ptr<Value> _value;

public:

	IDeclareConst(const std::string& constName, std::shared_ptr<Value> value) 
	{
		_constName = constName;
		_value = value;
	}
	~IDeclareConst() override {}

	void Execute(ProgramState& programState) override 
	{
		programState.valuesTable[_constName] = _value;

		programState.log.push_back("Const " + _constName + " = " + _value->PrintValue() + " added");
	};
};


// Registers a new variable in the value table
class IDeclareVar : public Instruction
{
private:
	Type _type;
	std::string _name;

public:

	IDeclareVar(Type type, const std::string& name) 
	{
		_type = type;
		_name = name;
	}
	~IDeclareVar() override {}

	void Execute(ProgramState& programState) override 
	{
		std::shared_ptr<Value> value;
		if (_type == INT) value = std::make_shared<IntValue>(0);
		if (_type == DOUBLE) value = std::make_shared<DoubleValue>(0.0f);
		if (_type == STRING) value = std::make_shared<StringValue>("\0");

		programState.valuesTable[_name] = value;
	}
};



// Modifes the value of the given variable
class IAssignVar : public Instruction
{

public:

	IAssignVar(const std::string& name, std::shared_ptr<Expression> expression) {}
	~IAssignVar() override {}

	void Execute(ProgramState& programState) override {};
};


// Reads input from keyboard, writing it into the given variable
class IRead : public Instruction
{

public:

	IRead(const std::string& varName) {}
	~IRead() override {}

	void Execute(ProgramState& programState) override {};
};


// Prints given values into the console
class IWrite : public Instruction
{

public:

	IWrite(std::vector<std::shared_ptr<Expression>> expressions) {}
	~IWrite() override {}

	void Execute(ProgramState& programState) override {};
};



// Branching function
class IIf : public Instruction
{

public:

	IIf(std::shared_ptr<Expression> expression) {}
	~IIf() override {}

	void Execute(ProgramState& programState) override {};
};


// Else function
class IElse : public Instruction
{

public:

	IElse() {}
	~IElse() override {}

	void Execute(ProgramState& programState) override {};
};


// While loop function
class IWhile : public Instruction
{

public:

	IWhile(std::shared_ptr<Expression> expression) {}
	~IWhile() override {}

	void Execute(ProgramState& programState) override {};
};