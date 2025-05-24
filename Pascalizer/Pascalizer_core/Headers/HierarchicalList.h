#pragma once
#include <memory>

/*
 * A non-encapsulated hierarchical list object
 * Will be used for storing program's source code
 */

template <typename T>
struct HListNode
{
	T value;

	// Pointer to the next element on the same level
	std::shared_ptr<HListNode<T>> pNext;

	// Pointer to the first element on the sub level
	std::shared_ptr<HListNode<T>> pSub;

	// Pointer to the parent element of this level
	std::shared_ptr<HListNode<T>> pUp;
};


template <typename T>
class HierarchicalList
{
	std::shared_ptr<HListNode<T>> pFirst;
	std::shared_ptr<HListNode<T>> pLast;
	
	size_t sz;

public:

	HierarchicalList();
	~HierarchicalList();

	size_t size() { return sz; }

	// Adds element on the same level as the last one
	void AddNextElement(T element);

	// Adds element to the sub level of the last one
	void AddSubElement(T element);

	// Adds element as the next element of the parent of this level
	void AddUpElement(T element);

	// Returns a POINTER to the first element of the list
	// Will be used for interpretation
	std::shared_ptr<HListNode<T>> GetFirst() { return pFirst; }


	const HierarchicalList& operator= (const HierarchicalList& rhs) { return *this; }
};