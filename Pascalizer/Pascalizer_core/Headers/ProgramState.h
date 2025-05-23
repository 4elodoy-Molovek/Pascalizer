#pragma once
#include <map>
#include "HierarchicalList.h"
#include "ValuesTable.h"

/*
 * Struct, containing all data about the current state state of the running program 
 */
struct ProgramState
{
	// Analyzed code of the whole program
	const HierarchicalList<std::shared_ptr<class Instruction>> code;

	// Table of values (constants and variable)
	std::map<std::string, std::shared_ptr<Value>> valuesTable;

	// Pointer to the currently executing instruction
	HListNode<class Instruction*>* instructionPointer;

	// Program execution log
	std::vector<std::string> log;

<<<<<<< Updated upstream
	ProgramState(const HierarchicalList<class Instruction*>& inCode) : code(inCode) {}
=======
	// Caches condition results for 'else' statements
	std::stack<bool> branchingStack;

	ProgramState(const HierarchicalList<std::shared_ptr<class Instruction>>& inCode) : code(inCode) {}
>>>>>>> Stashed changes
};