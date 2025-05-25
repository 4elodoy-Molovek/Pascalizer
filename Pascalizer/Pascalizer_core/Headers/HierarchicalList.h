#pragma once
#include <memory>

/*
 * A non-encapsulated hierarchical list object
 * Will be used for storing program's code
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


	// Destroys the links between the elements, to insure their deletion
	void UnlinkNode(std::shared_ptr<HListNode<T>> node)
	{
		if (node->pNext) UnlinkNode(node->pNext);
		if (node->pSub) UnlinkNode(node->pSub);

		node->pNext = nullptr;
		node->pSub = nullptr;
		node->pUp = nullptr;
	}

public:

	HierarchicalList() : sz(0) {}
	~HierarchicalList() {}

	size_t size() { return sz; }

	// Adds element on the same level as the last one
	void AddNextElement(const T& element)
	{
		if (!pFirst)
		{
			pFirst = pLast = std::make_shared<HListNode<T>>(element);
			sz++;

			return;
		}

		pLast->pNext = std::make_shared<HListNode<T>>(element);
		pLast = pLast->pNext;
		
		sz++;
	}

	// Adds element to the sub level of the last one
	void AddSubElement(const T& element)
	{
		if (!pFirst)
		{
			pFirst = pLast = std::make_shared<HListNode<T>>(element);
			sz++;

			return;
		}

		pLast->pSub = std::make_shared<HListNode<T>>(element);
		pLast = pLast->pSub;

		sz++;
	}

	// Adds element as the next element of the parent of this level
	void AddUpElement(const T& element)
	{
		if (!pFirst)
		{
			pFirst = pLast = std::make_shared<HListNode<T>>(element);
			sz++;

			return;
		}

		pLast->pUp = std::make_shared<HListNode<T>>(element);
		pLast = pLast->pUp;

		sz++;
	}

	// Returns a POINTER to the first element of the list
	// Will be used for interpretation
	std::shared_ptr<HListNode<T>> GetFirst() { return pFirst; }

	// Makes the list empty
	void Clear()
	{
		if (pFirst)
		{
			UnlinkNode(pFirst);

			pFirst = nullptr;
			pLast = nullptr;
			
			sz = 0;
		}
	}


	const HierarchicalList& operator= (const HierarchicalList& rhs) { return *this; }
};