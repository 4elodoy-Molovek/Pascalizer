#pragma once
#include <map>
#include <stack>
#include "HierarchicalList.h"
#include "ValuesTable.h"

/*
 * Struct, containing all data about the current state state of the running program 
 */
struct ProgramState
{
	// Analyzed code of the whole program
	const HierarchicalList<std::shared_ptr<Instruction>> code;

	// Table of values (constants and variable)
	std::map<std::string, std::shared_ptr<Value>> valuesTable;

	// Pointer to the currecntly executing instruction
	std::shared_ptr<HListNode<std::shared_ptr<class Instruction>>> currentInstruction;

	// Pointer to the instruction that is to be executed next
	std::shared_ptr<HListNode<std::shared_ptr<class Instruction>>> instructionPointer;

	// Program execution log
	std::vector<std::string> log;

	// Caches condition results for 'else' statements
	std::stack<bool> branchingStack;


	ProgramState(const HierarchicalList<class Instruction*>& inCode) : code(inCode) {}
};