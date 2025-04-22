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
	const HierarchicalList<class Instruction*> code;

	// Table of values (constants and variable)
	std::map<std::string, Value> valuesTable;

	// Pointer to the currently executing instruction
	HListNode<class Instruction*>* instructionPointer;

	// Program execution log
	std::vector<std::string> log;

	ProgramState(const HierarchicalList<class Instruction*>& inCode) : code(inCode) {}
};