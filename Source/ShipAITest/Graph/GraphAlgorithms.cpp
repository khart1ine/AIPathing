// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphAlgorithms.h"
#include "Engine.h"

bool Graph_SearchDFS::Search()
{
	//create a std stack of edges
	TArray<const NavGraphEdge*> Stack;

	//create a dummy edge and put on the stack
	NavGraphEdge Dummy(Source, Source, 0);

	//const NavGraphEdge* Next = nullptr;
	const NavGraphEdge* pE = nullptr;

	Stack.Push(&Dummy);

	//while there are edges in the stack keep searching
	while (Stack.Num() != 0)
	{
		//grab the next edge
		const NavGraphEdge* Next = Stack.Top();

		//make a note of the parent of the node this edge points to
		Route[Next->GetTo()] = Next->GetFrom();
		
		//remove the edge from the stack
		Stack.Pop();

		//put it on the tree. (making sure the dummy edge is not placed on the tree)
		if (Next != &Dummy)
		{
			SpanningTree.Push(Next);
		}

		//and mark it visited
		NodesVisited[Next->GetTo()] = visited;

		//if the target has been found the method can return success
		if (Next->GetTo() == Target)
		{
			return true;
		}

		//push the edges leading from the node this edge points to onto
		//the stack (provided the edge does not point to a previously 
		//visited node)
		SparseGraph::ConstEdgeIterator ConstEdgeItr(Graph, Next->GetTo());

		for (pE = ConstEdgeItr.begin();
			!ConstEdgeItr.end();
			pE = ConstEdgeItr.next())
		{
			if (NodesVisited[pE->GetTo()] == unvisited)
			{
				Stack.Push(pE);
			}
		}
	}

	//no path to target
	return false;
}

TArray<int32> Graph_SearchDFS::GetPathToTarget()const
{
	TArray<int32> path;

	//just return an empty path if no path to target found or if
	//no target has been specified
	if (!PathFound || Target < 0) return path;

	int nd = Target;

	path.Insert(nd, 0);

	while (nd != Source)
	{
		nd = Route[nd];

		path.Insert(nd, 0);
	}

	return path;
}
