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


static std::string strType[3] = { "INT", "DOUBLE", "STRING" }; //for logs and exceptions

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

	std::string GetStringNotation() override
	{
		return "IProgram(" + _programName + ")";
	}
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
		programState.codeBlockExitInstructionPointerStack.push(programState.currentInstruction->pNext);
	};

	std::string GetStringNotation() override
	{
		return "IConstBlock()";
	}
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
		programState.codeBlockExitInstructionPointerStack.push(programState.currentInstruction->pNext);
	};

	std::string GetStringNotation() override
	{
		return "IVarBlock()";
	}
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

	std::string GetStringNotation() override
	{
		return "IMainBlock()";
	}
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

	std::string GetStringNotation() override
	{
		return "IDeclareConst(" + _constName + ", " + _value->PrintValue() + ")";
	}
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

	std::string GetStringNotation() override
	{
		return "IDeclareVar(" + strType[_type] + ", " + _name + ")";
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
			value = _expression->Calculate(programState);
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
			throw(std::runtime_error("FATAL: tried to assign " + strType[programState.valuesTable[_name]->GetType()] + " to " + strType[value->GetType()]));
		}

		programState.valuesTable[_name] = value;
	}

	std::string GetStringNotation() override
	{
		return "IAssignVar(" + _name + ")";
	}

	std::string GetExpressionNotation()
	{
		return _expression->GetStringPostfix();
	}
};


// Reads input from keyboard, writing it into the given variable
class IRead : public Instruction, public IO_InstructionInterface
{
private:
	std::shared_ptr<Expression> _varNameExpression;

	std::string cachedVarName;
	ProgramState* programStatePtr;


	bool IsInt(const std::string& str)
	{
		for (int i = 0; i < str.length(); i++)
			if (!std::isdigit(str[i]) && !(str[i] == '-' && i == 0))
				return false;

		return true;
	}

	bool IsDouble(const std::string& str)
	{
		for( int i = 0; i < str.length(); i++)
			if (!std::isdigit(str[i]) && str[i] != '.' && !(str[i] == '-' && i == 0))
				return false;

		return true;
	}

public:

	IRead(std::shared_ptr<Expression> varNameExpression)
	{
		_varNameExpression = varNameExpression;
	}
	~IRead() override {}

	void Execute(ProgramState& programState) override
	{
		// Calculating var name expression
		std::shared_ptr<Value> nameValue;
		programState.log.push_back("Trying to calculate expression...");
		try
		{
			nameValue = _varNameExpression->Calculate(programState);
		}
		catch (std::runtime_error e)
		{
			programState.log.push_back(e.what());
			programState.log.push_back("FATAL: Calculation failed");
			throw(std::runtime_error("FATAL: Calculation failed"));
		}

		if (nameValue->GetType() != STRING) throw(std::runtime_error("FATAL: Read instruction received a non-string variable name"));

		cachedVarName = dynamic_cast<StringValue*>(nameValue.get())->value;

		if (programState.valuesTable.count(cachedVarName) == 0) throw(std::runtime_error("FATAL: Read instruction trying to write into an invalid variable '" + cachedVarName + "'!"));


		programStatePtr = &programState;

		programState.log.push_back("Reading from standard input...");

		programState.ioBlock = true;
		programState.ioProcessor->CallReceiveUserInput(this);
	}

	// Called when the user input has been received after this instruction requested it
	virtual void OnUserInputReceived(const std::string& userInput) override
	{
		programStatePtr->ioBlock = false;

		std::string input = userInput;
		std::shared_ptr<Value> value;
	
		
		if (IsInt(input)) // if got an int
		{
			int intInput = std::stoi(input);

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
		else if (IsDouble(input)) // if got a double
		{
			double doubleInput = std::stod(input);
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

	std::string GetStringNotation() override
	{
		return "IRead()";
	}

	std::string GetExpressionNotation()
	{
		return _varNameExpression->GetStringPostfix();
	}
};


// Prints given values into the console
class IWrite : public Instruction
{
private:
	std::vector<std::shared_ptr<Expression>> _expressions;

public:

	IWrite(std::vector<std::shared_ptr<Expression>> expressions)
	{
		_expressions = expressions;
	}
	~IWrite() override {}

	void Execute(ProgramState& programState) override
	{
		std::string output;

		programState.log.push_back("Writing to standard output...");

		programState.log.push_back("Trying to calculate expressions...");
		int expressionCount = _expressions.size();
		std::shared_ptr<Expression> exp;
		std::shared_ptr<Value> value;
		for (int i = 0; i < expressionCount; i++)
		{
			programState.log.push_back("Calculating expression " + std::to_string(i + 1) + "/" + std::to_string(expressionCount));
			exp = _expressions[i];

			try
			{
				value = exp->Calculate(programState);
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

	std::string GetStringNotation() override
	{
		return "IWrite()";
	}

	std::string GetExpressionNotation()
	{
		std::string result = "";
		for (auto& expr : _expressions)
			result += expr->GetStringPostfix() + ", ";

		return result;
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
			programState.instructionPointer = programState.currentInstruction->pNext;
		}
		else
		{
			programState.log.push_back("Entering ELSE body");
			programState.codeBlockExitInstructionPointerStack.push(programState.currentInstruction->pNext);
		}
	}

	std::string GetStringNotation() override
	{
		return "IElse()";
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
			result = condition->Calculate(programState);
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

		auto currentNode = programState.currentInstruction;
		bool hasElse = currentNode->pNext && std::dynamic_pointer_cast<IElse>(currentNode->pNext->value);

		if (hasElse)
		{
			programState.log.push_back("IF block has ELSE");
			programState.branchingStack.push(isTrue);
		}

		if (isTrue)
		{
			programState.log.push_back("Entering IF body");
			programState.codeBlockExitInstructionPointerStack.push(programState.currentInstruction->pNext);
		}
		else
		{
			programState.log.push_back("Skipping IF body");
			programState.instructionPointer = currentNode->pNext;
		}
	}

	std::string GetStringNotation() override
	{
		return "IIf()";
	}

	std::string GetExpressionNotation()
	{
		return condition->GetStringPostfix();
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
			result = condition->Calculate(programState);
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

			programState.codeBlockExitInstructionPointerStack.push(programState.currentInstruction);
		}
		else
		{
			programState.log.push_back("Skipping WHILE body");
			programState.instructionPointer = programState.currentInstruction->pNext;
		}
	}

	std::string GetStringNotation() override
	{
		return "IWhile()";
	}

	std::string GetExpressionNotation()
	{
		return condition->GetStringPostfix();
	}
};