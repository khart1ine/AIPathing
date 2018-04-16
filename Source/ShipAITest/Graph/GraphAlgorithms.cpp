// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphAlgorithms.h"

bool Graph_SearchDFS::Search()
{
	TArray<const NavGraphEdge*> Stack;

	NavGraphEdge Dummy(Source, Source, 0);

	Stack.Push(&Dummy);

	const NavGraphEdge* pE = nullptr;

	while (Stack.Num() != 0)
	{
		const NavGraphEdge* Next = Stack.Top();

		Stack.Pop();

		Route[Next->GetTo()] = Next->GetFrom();

		if (Next != &Dummy)
		{
			SpanningTree.Push(Next);
		}

		NodesVisited[Next->GetTo()] = visited;

		if (Next->GetTo() == Target)
		{
			return true;
		}

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

	return false;
}

TArray<int32> Graph_SearchDFS::GetPathToTarget()const
{
	TArray<int32> path;

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
