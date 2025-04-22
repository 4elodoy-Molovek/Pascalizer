#pragma once
#include <string>

// Possible value types
enum ValueType {INT, DOUBLE, STRING};

// Struct, containg data about a single value
struct Value
{
	// Type of the value
	ValueType type;

	// Value itself
	void* value;
};

/*
 * Table module, containing values of all constatans and variables of the running program 
 */
class ValuesTable
{
	/*
	 * Some data storage here 
	 */

public:
	
	ValuesTable();
	~ValuesTable();

	// Adds a new value to the table
	void AddElement(const std::string& key, Value value);

	// Removes a value from the table
	void RemoveElement(const std::string& key);

	// Finds a value in the table, if value does not exist: returns 'nullptr'
	Value* GetElement(const std::string& key);
};