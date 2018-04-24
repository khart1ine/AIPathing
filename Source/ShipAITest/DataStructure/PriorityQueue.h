// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//----------------------- IndexedPriorityQLow ---------------------------
//
//  Priority queue based on an index into a set of keys. The queue is
//  maintained as a 2-way heap.
//
//  The priority in this implementation is the lowest valued key
//------------------------------------------------------------------------
template<class KeyType>
class SHIPAITEST_API IndexedPriorityQLow
{

private:

	TArray<KeyType>& VecKeys;

	TArray<int> Heap;

	TArray<int> InvHeap;

	int ISize, IMaxSize;

	void Swap(int a, int b)
	{
		int temp = Heap[a]; Heap[a] = Heap[b]; Heap[b] = temp;

		//change the handles too
		InvHeap[Heap[a]] = a; InvHeap[Heap[b]] = b;
	}

	void ReorderUpwards(int nd)
	{
		//move up the heap swapping the elements until the heap is ordered
		while ((nd>1) && (VecKeys[Heap[nd / 2]] > VecKeys[Heap[nd]]))
		{
			Swap(nd / 2, nd);

			nd /= 2;
		}
	}
	void ReorderDownwards(int nd, int HeapSize)
	{
		//move down the heap from node nd swapping the elements until
		//the heap is reordered
		while (2 * nd <= HeapSize)
		{
			int child = 2 * nd;

			//set child to smaller of nd's two children
			if ((child < HeapSize) && (VecKeys[Heap[child]] > VecKeys[Heap[child + 1]]))
			{
				++child;
			}

			//if this nd is larger than its child, swap
			if (VecKeys[Heap[nd]] > VecKeys[Heap[child]])
			{
				Swap(child, nd);

				//move the current node down the tree
				nd = child;
			}

			else
			{
				break;
			}
		}
	}



public:
	//you must pass the constructor a reference to the std::vector the PQ
	//will be indexing into and the maximum size of the queue.
	IndexedPriorityQLow(TArray<KeyType>& keys,
		int MxSize) :VecKeys(keys),
		IMaxSize(MxSize),
		ISize(0)
	{
		Heap.Init(0, IMaxSize + 1);
		InvHeap.Init(0, IMaxSize + 1);
	}

	bool IsEmpty()const { return (ISize == 0); }

	//to insert an item into the queue it gets added to the end of the heap
	//and then the heap is reordered from the bottom up.
	void Insert(const int idx)
	{
		check(ISize + 1 <= IMaxSize);

		++ISize;

		Heap[ISize] = idx;

		InvHeap[idx] = ISize;

		ReorderUpwards(ISize);
	}

	//to get the min item the first element is exchanged with the lowest
	//in the heap and then the heap is reordered from the top down. 
	int32 Pop()
	{
		Swap(1, ISize);

		ReorderDownwards(1, ISize - 1);

		return Heap[ISize--];
	}

	//if the value of one of the client key's changes then call this with 
	//the key's index to adjust the queue accordingly
	void ChangePriority(const int idx)
	{
		ReorderUpwards(InvHeap[idx]);
	}
};
