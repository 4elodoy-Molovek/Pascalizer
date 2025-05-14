#pragma once

/*
 * A non-encapsulated hierarchical list object
 * Will be used for storing program's source code
 */

template <typename T>
struct HListNode
{
	T value;

	// Pointer to the next element on the same level
	HListNode<T>* pNext;

	// Pointer to the first element on the sub level
	HListNode<T>* pSub;

	// Pointer to the parent element of this level
	HListNode<T>* pUp;
};


template <typename T>
class HierarchicalList
{
	HListNode<T>* pFirst;
	HListNode<T>* pLast;
	
	size_t sz;

public:

	HierarchicalList();
	~HierarchicalList();

	size_t size() { return sz; }

	// Adds element on the same level as the last one
	void AddNextElement();

	// Adds element to the sub level of the last one
	void AddSubElement();

	// Adds element as the next element of the parent of this level
	void AddUpElement();

	// Returns a POINTER to the first element of the list
	// Will be used for interpretation
	HListNode<T>* GetFirst() { return pFirst; }


	const HierarchicalList& operator= (const HierarchicalList& rhs) { return *this; }
};