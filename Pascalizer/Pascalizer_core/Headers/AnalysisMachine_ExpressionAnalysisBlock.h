#pragma once
#include "Tokens.h"
#include "AnalysisMachine.h"
#include "ExpressionEvaluationBlock.h"
#include <set>

/*
 * Processes any expression expected in the code
 */
class ExpressionAnalysisBlockState : public State
{
	// Inner state of the implicit state machine
	std::string innerState = "initial";


	// Storage
	std::vector<std::shared_ptr<ExpressionElement>> postfix;
	std::stack<std::string> operationStack;

	std::string lastName;
	int bracketCounter = 0;

	// Adds a value to the postfix
	void AddValue(std::shared_ptr<Value> value) 
	{
		postfix.push_back(std::make_shared<ValueExpressionElement>(value));
	}

	// Adds a value to the postfix
	void AddVariable(const std::string& varName)
	{
		postfix.push_back(std::make_shared<VariableExpressionElement>(varName));
	}

	// Pushes operation into the operation stack
	void PushOperation(const std::string& operationStr) 
	{
		operationStack.push(operationStr);
	}

	// Dumps the operation stack into postfix until it meets the break point
	void DumpOperationStack() 
	{
		const std::set<std::string> breakPoints = { "(", "sin" };
		while (operationStack.size() > 0 && breakPoints.count(operationStack.top()) == 0)
		{
			postfix.push_back(CreateOperationElement(operationStack.top()));
			operationStack.pop();
		}

		if (operationStack.size() > 0)
		{
			// Pushing break point operation (ignoring '(')
			if (operationStack.top() != "(") postfix.push_back(CreateOperationElement(operationStack.top()));
			operationStack.pop();
		}
	}

	//
	std::shared_ptr<ExpressionElement> CreateOperationElement(const std::string& operationStr)
	{
		std::shared_ptr<OperationExpressionElement> operation;

				if (operationStr == "+") operation = std::make_shared<AddOperation>();
		else	if (operationStr == "-") operation = std::make_shared<SubtractOperation>();
		else	if (operationStr == "*") operation = std::make_shared<MultiplyOperation>();
		else	if (operationStr == "/") operation = std::make_shared<DivideOperation>();
		else	if (operationStr == "mod") operation = std::make_shared<ModOperation>();
		else	if (operationStr == "div") operation = std::make_shared<DivOperation>();

		else	if (operationStr == ">") operation = std::make_shared<MoreOperation>();
		else	if (operationStr == ">=") operation = std::make_shared<MoreEqualOperation>();
		else	if (operationStr == "<") operation = std::make_shared<LessOperation>();
		else	if (operationStr == "<=") operation = std::make_shared<LessEqualOperation>();
		else	if (operationStr == "==") operation = std::make_shared<EqualOperation>();
		else	if (operationStr == "!=") operation = std::make_shared<NotEqualOperation>();

		
		// Functions (aslo processed as math operators for now)
		else	if (operationStr == "sin") operation = std::make_shared<SinOperation>();

		return operation;
	}

	//
	void ExitProcedure(const Token& exitElement)
	{
		std::shared_ptr<Expression> result = std::make_shared<Expression>(Expression(postfix));
		parentMachine.ExpressionAnalysisFinished(result, exitElement);
	}


public:
	ExpressionAnalysisBlockState(const AnalysisMachine& analysisMachine) : State(analysisMachine) {}


	~ExpressionAnalysisBlockState() {}

	// Called when the machine enters this state by traversing a connection based on the TokenizedElement 'element'
	virtual void EnterState(const Token& element) override
	{
		// Resetting the inner state
		innerState = "initial";

		// Cleaning the storage
		postfix.clear();
		while (operationStack.size() > 0) operationStack.pop(); // Just in case
		lastName = "";
		bracketCounter = 0;

	}

	// Called when the machine exits this state
	virtual void ExitState() override {}

	virtual State* ProcessElement(const Token& nextElement) override 
	{
		// Base loop state
		if (innerState == "initial")
		{
			if (nextElement.type == NAME)
			{
				lastName = nextElement.value;
				innerState = "name";

				return nullptr;
			}

			if (nextElement.type == VALUE_INT || nextElement.type == VALUE_DOUBLE || nextElement.type == VALUE_STRING)
			{
				// Creating value from the contents
				std::shared_ptr<Value> newValue;

						if (nextElement.type == VALUE_INT) newValue = std::make_shared<IntValue>(std::stoi(nextElement.value));
				else	if (nextElement.type == VALUE_DOUBLE) newValue = std::make_shared<DoubleValue>(std::stod(nextElement.value));
				else	if (nextElement.type == VALUE_STRING) newValue = std::make_shared<StringValue>(nextElement.value);

				AddValue(newValue);

				innerState = "reloop";
				return nullptr;
			}

			if (nextElement.type == MATH_OPERATOR)
			{
				PushOperation(nextElement.value);

				innerState = "reloop";
				return nullptr;
			}

			if (nextElement.type == BRACKET_OPEN)
			{
				PushOperation(nextElement.value);
				bracketCounter++;

				innerState = "reloop";
				return nullptr;
			}

			if (nextElement.type == COMMA || nextElement.type == END_LINE)
			{
				ExitProcedure(nextElement);
				return nullptr;
			}
		}

		// Value state
		if (innerState == "name")
		{
			// Functions
			if (nextElement.type == BRACKET_OPEN)
			{
				const std::set<std::string> functionNames = { "sin" }; // To be replaced in case of expansion

				// Checking if function is valid
				if (functionNames.count(lastName) == 0) throw (std::exception("ANALYSIS ERROR: Unknown funtion in expression!"));

				PushOperation(lastName);

				bracketCounter++;

				innerState = "reloop";
				return nullptr;
			}

			// Variables
			else
			{
				AddVariable(lastName);
				innerState = "reloop";
				return nullptr;
			}
		}


		if (innerState == "reloop")
		{
			if (nextElement.type == BRACKET_CLOSE)
			{
				if (bracketCounter > 0)
				{
					bracketCounter--;
					DumpOperationStack();
					innerState = "initial";
					return nullptr;
				}

				else
				{
					ExitProcedure(nextElement);
					return nullptr;
				}
			}
		}
	}
};