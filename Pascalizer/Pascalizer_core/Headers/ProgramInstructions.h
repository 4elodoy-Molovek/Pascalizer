#pragma once
#include "Instruction.h"
#include "ExpressionEvaluationBlock.h"
#include "ValuesTable.h"
#include <cmath>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include "IO_ProcessorInterface.h"


std::string strType[3] = { "INT", "DOUBLE", "STRING" }; //for logs and exceptions

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
		programState.log.push_back("Declaring const...");
		programState.valuesTable[_constName] = _value;
		programState.log.push_back("Const " + _constName + " = " + _value->PrintValue() + " declared");
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
		programState.log.push_back("Declaring var...");
		std::shared_ptr<Value> value;
		if (_type == INT) value = std::make_shared<IntValue>(0);
		if (_type == DOUBLE) value = std::make_shared<DoubleValue>(0.0f);
		if (_type == STRING) value = std::make_shared<StringValue>("\0");

		programState.valuesTable[_name] = value;

		programState.log.push_back("Var " + _name + " = " + value->PrintValue() + " declared");
	}
};



// Modifes the value of the given variable
class IAssignVar : public Instruction
{
private:
	std::string _name;
	std::shared_ptr<Expression> _expression;

public:

	IAssignVar(const std::string& name, std::shared_ptr<Expression> expression)
	{
		_name = name;
		_expression = expression;
	}
	~IAssignVar() override {}

	void Execute(ProgramState& programState) override
	{
		programState.log.push_back("Assigning var...");
		programState.log.push_back("Trying to calculate expression...");
		std::shared_ptr<Value> value;

		try
		{
			value = _expression->Caculate(programState);
		}
		catch (std::runtime_error e)
		{
			programState.log.push_back(e.what());
			programState.log.push_back("FATAL: Calculation failed");
			throw(std::runtime_error("FATAL: Calculation failed"));
		}
		catch (std::exception e)
		{
			programState.log.push_back("ERROR: " + std::string(e.what()));
			programState.log.push_back("FATAL: Calculation failed");
			throw(std::runtime_error("FATAL: Calculation failed"));
		}

		programState.log.push_back("Calculation succeded");

		if (programState.valuesTable[_name]->GetType() != value->GetType())
		{
			programState.log.push_back("FATAL: tried to assign " + strType[programState.valuesTable[_name]->GetType()] + " to " + strType[value->GetType()]);
			throw("FATAL: tried to assign " + strType[programState.valuesTable[_name]->GetType()] + " to " + strType[value->GetType()]);
		}

		programState.valuesTable[_name] = value;
	}
};


// Reads input from keyboard, writing it into the given variable
class IRead : public Instruction, public IO_InstructionInterface
{
private:
	std::shared_ptr<Expression> _varNameExpression;

	std::string cachedVarName;
	ProgramState* programStatePtr;

public:

	IRead(std::shared_ptr<Expression> varNameExpression)
	{
		_varNameExpression = varNameExpression;
	}
	~IRead() override {}

	void Execute(ProgramState& programState) override
	{
		// Calculating var name expression
		std::shared_ptr<Value> nameValue = _varNameExpression->Caculate(programState);

		if (nameValue->GetType() != STRING) throw(std::runtime_error("FATAL: Read instruction received a non-string variable name"));

		std::string cachedVarName = std::dynamic_pointer_cast<StringValue>(nameValue)->value;

		if (programState.valuesTable.count(cachedVarName) == 0) throw(std::runtime_error("FATAL: Read instruction trying to write into an invalid variable '" + cachedVarName + "'!"));


		programStatePtr = &programState;

		programState.log.push_back("Reading from standard input...");

		programState.ioProcessor->CallReceiveUserInput(this);
	}

	// Called when the user input has been received after this instruction requested it
	virtual void OnUserInputReceived(const std::string& userInput) override
	{
		std::string input;
		std::shared_ptr<Value> value;

		size_t intPos;
		size_t doublePos;
		int intInput = std::stoi(input, &intPos);
		double doubleInput = std::stod(input, &doublePos);
		if (intPos == input.size()) // if got an int
		{
			programStatePtr->log.push_back("Got INT: " + input);
			if (programStatePtr->valuesTable[cachedVarName]->GetType() == Type::INT)
			{
				value = std::make_shared<IntValue>(intInput);
				programStatePtr->valuesTable[cachedVarName] = value;
				programStatePtr->log.push_back("Assigned to var " + cachedVarName);
			}
			else if (programStatePtr->valuesTable[cachedVarName]->GetType() == Type::DOUBLE)
			{
				value = std::make_shared<DoubleValue>(intInput);
				programStatePtr->valuesTable[cachedVarName] = value;
				programStatePtr->log.push_back("Assigned to var " + cachedVarName);
			}
			else
			{
				programStatePtr->log.push_back("FATAL: tried to assign INT to " + strType[programStatePtr->valuesTable[cachedVarName]->GetType()]);
				throw(std::runtime_error("FATAL: tried to assign INT to " + strType[programStatePtr->valuesTable[cachedVarName]->GetType()]));
			}

		}
		else if (doublePos == input.size()) // if got a double
		{
			programStatePtr->log.push_back("Got DOUBLE: " + input);
			if (programStatePtr->valuesTable[cachedVarName]->GetType() == Type::DOUBLE)
			{
				value = std::make_shared<DoubleValue>(doubleInput);
				programStatePtr->valuesTable[cachedVarName] = value;
				programStatePtr->log.push_back("Assigned to var " + cachedVarName);
			}
			else if (programStatePtr->valuesTable[cachedVarName]->GetType() == Type::INT)
			{
				value = std::make_shared<IntValue>(std::round(doubleInput));
				programStatePtr->valuesTable[cachedVarName] = value;
				programStatePtr->log.push_back("Assigned to var " + cachedVarName);
			}
			else
			{
				programStatePtr->log.push_back("FATAL: tried to assign DOUBLE to " + strType[programStatePtr->valuesTable[cachedVarName]->GetType()]);
				throw(std::runtime_error("FATAL: tried to assign DOUBLE to " + strType[programStatePtr->valuesTable[cachedVarName]->GetType()]));
			}
		}
		else // if got a string
		{
			programStatePtr->log.push_back("Got STRING");
			if (programStatePtr->valuesTable[cachedVarName]->GetType() == Type::STRING)
			{
				value = std::make_shared<StringValue>(input);
				programStatePtr->valuesTable[cachedVarName] = value;
				programStatePtr->log.push_back("Assigned to var " + cachedVarName);
			}
			else
			{
				programStatePtr->log.push_back("FATAL: tried to assign DOUBLE to " + strType[programStatePtr->valuesTable[cachedVarName]->GetType()]);
				throw(std::runtime_error("FATAL: tried to assign DOUBLE to " + strType[programStatePtr->valuesTable[cachedVarName]->GetType()]));
			}
		}
	}
};


// Prints given values into the console
class IWrite : public Instruction
{
private:
	std::vector<std::shared_ptr<Expression>> _expressions;
	std::string output;

public:

	IWrite(std::vector<std::shared_ptr<Expression>> expressions)
	{
		_expressions = expressions;
	}
	~IWrite() override {}

	void Execute(ProgramState& programState) override
	{
		programState.log.push_back("Writing to standard output...");

		programState.log.push_back("Trying to calculate expressions...");
		int expressionCount = _expressions.size();
		std::shared_ptr<Expression> exp;
		std::shared_ptr<Value> value;
		for (int i = 0; i < expressionCount; i++)
		{
			programState.log.push_back("Calculating expression " + std::to_string(i) + "/" + std::to_string(expressionCount));
			exp = _expressions[i];

			try
			{
				value = exp->Caculate(programState);
			}
			catch (std::runtime_error e)
			{
				programState.log.push_back(e.what());
				programState.log.push_back("FATAL: Expression " + std::to_string(i) + "/" + std::to_string(expressionCount) + " calculation failed");
				throw(std::runtime_error("FATAL: Expression " + std::to_string(i) + "/" + std::to_string(expressionCount) + " calculation failed"));
			}
			catch (std::exception e)
			{
				programState.log.push_back("ERROR: " + std::string(e.what()));
				programState.log.push_back("FATAL: Expression " + std::to_string(i) + "/" + std::to_string(expressionCount) + " calculation failed");
				throw(std::runtime_error("FATAL: Expression " + std::to_string(i) + "/" + std::to_string(expressionCount) + " calculation failed"));
			}

			output += value->PrintValue();
		}

		programState.log.push_back("OUTPUT: " + output);
		programState.ioProcessor->CallOutputString(output);
	}
};


// Else function
class IElse : public Instruction
{

public:

	IElse() {}
	~IElse() override {}

	void Execute(ProgramState& programState) override
	{
		programState.log.push_back("ELSE block started");

		if (programState.branchingStack.empty())
		{
			programState.log.push_back("FATAL: ELSE without mathing IF");
			throw(std::runtime_error("FATAL: ELSE without mathing IF"));
		}

		bool ifConditionWasTrue = programState.branchingStack.top();
		programState.branchingStack.pop();

		std::string ifConditionWasTrueStr = ifConditionWasTrue ? "TRUE" : "FALSE";
		programState.log.push_back("Condition is: " + ifConditionWasTrueStr);

		if (ifConditionWasTrue)
		{
			programState.log.push_back("Skipping ELSE body");
			programState.instructionPointer = programState.instructionPointer->pNext;
		}
		else
		{
			programState.log.push_back("Entering ELSE body");
		}
	}
};


// Branching function
class IIf : public Instruction
{
private:
	std::shared_ptr<Expression> condition;

public:

	IIf(std::shared_ptr<Expression> expression)
	{
		condition = expression;
	}
	~IIf() override {}

	void Execute(ProgramState& programState) override
	{
		programState.log.push_back("IF block started");

		programState.log.push_back("Trying to calculate condition...");
		std::shared_ptr<Value> result;

		try
		{
			result = condition->Caculate(programState);
		}
		catch (std::runtime_error e)
		{
			programState.log.push_back(e.what());
			programState.log.push_back("FATAL: Calculation failed");
			throw(std::runtime_error("FATAL: Calculation failed"));
		}
		catch (std::exception e)
		{
			programState.log.push_back("ERROR: " + std::string(e.what()));
			programState.log.push_back("FATAL: Calculation failed");
			throw(std::runtime_error("FATAL: Calculation failed"));
		}

		programState.log.push_back("Calculation succeded");

		bool isTrue = false;
		if (auto intVal = std::dynamic_pointer_cast<IntValue>(result))
			isTrue = intVal->value != 0;
		else if (auto doubleVal = std::dynamic_pointer_cast<DoubleValue>(result))
			isTrue = doubleVal->value != 0.0;
		else if (auto boolVal = std::dynamic_pointer_cast<StringValue>(result))
			isTrue = boolVal->value != "";

		std::string isTrueStr = isTrue ? "TRUE" : "FALSE";
		programState.log.push_back("Condition is: " + isTrueStr);

		auto currentNode = programState.instructionPointer;
		bool hasElse = currentNode->pNext && std::dynamic_pointer_cast<IElse>(currentNode->pNext->value);

		if (hasElse)
		{
			programState.log.push_back("IF block has ELSE");
			programState.branchingStack.push(isTrue);
		}

		if (isTrue)
		{
			programState.log.push_back("Entering IF body");
		}
		else
		{
			programState.log.push_back("Skipping IF body");
			programState.instructionPointer = currentNode->pNext;
		}
	}
};


// While loop function
class IWhile : public Instruction
{
private:
	std::shared_ptr<Expression> condition;

public:

	IWhile(std::shared_ptr<Expression> expression)
	{
		condition = expression;
	}
	~IWhile() override {}

	void Execute(ProgramState& programState) override
	{
		programState.log.push_back("WHILE block started");

		programState.log.push_back("Trying to calculate condition...");
		std::shared_ptr<Value> result;

		try
		{
			result = condition->Caculate(programState);
		}
		catch (std::runtime_error e)
		{
			programState.log.push_back(e.what());
			programState.log.push_back("FATAL: Calculation failed");
			throw(std::runtime_error("FATAL: Calculation failed"));
		}
		catch (std::exception e)
		{
			programState.log.push_back("ERROR: " + std::string(e.what()));
			programState.log.push_back("FATAL: Calculation failed");
			throw(std::runtime_error("FATAL: Calculation failed"));
		}

		programState.log.push_back("Calculation succeded");

		bool isTrue = false;
		if (auto intVal = std::dynamic_pointer_cast<IntValue>(result))
			isTrue = intVal->value != 0;
		else if (auto doubleVal = std::dynamic_pointer_cast<DoubleValue>(result))
			isTrue = doubleVal->value != 0.0;
		else if (auto boolVal = std::dynamic_pointer_cast<StringValue>(result))
			isTrue = boolVal->value != "";

		std::string isTrueStr = isTrue ? "TRUE" : "FALSE";
		programState.log.push_back("Condition is: " + isTrueStr);

		if (isTrue)
		{
			programState.log.push_back("Entering WHILE body");

			//preparing return to beginning after body
			auto loopStart = programState.instructionPointer;
			auto lastChild = loopStart->pSub;

			//finding last body instruction
			while (lastChild && lastChild->pNext)
				lastChild = lastChild->pNext;

			//returning to loop beginning
			if (lastChild)
				lastChild->pNext = loopStart;
		}
		else
		{
			programState.log.push_back("Skipping WHILE body");
			programState.instructionPointer = programState.instructionPointer->pNext;
		}
	}
};