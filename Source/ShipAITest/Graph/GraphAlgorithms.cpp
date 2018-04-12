// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphAlgorithms.h"
#include "Engine.h"

bool Graph_SearchDFS::Search()
{
	TArray<const NavGraphEdge*> Stack;

	NavGraphEdge Dummy(Source, Source, 0);

	const NavGraphEdge* Next = nullptr;

	Stack.Emplace(&Dummy);

	while (Stack.Num() != 0)
	{
		Next = Stack.Top();

		Stack.Pop(false);

		Route[Next->GetTo()] = Next->GetFrom();

		if (Next != &Dummy)
		{
			SpanningTree.Emplace(Next);
		}

		NodesVisited[Next->GetTo()] = visited;

		if (Next->GetTo() == Target)
		{
			return true;
		}

		SparseGraph::ConstEdgeIterator ConstEdgeItr(Graph, Next->GetTo());

		for (const NavGraphEdge* pE = ConstEdgeItr.begin();
			!ConstEdgeItr.end();
			pE = ConstEdgeItr.next())
		{
			if (NodesVisited[pE->GetTo()] == unvisited)
			{
				Stack.Emplace(pE);
			}
		}
		int temp = Stack.Num();
		UE_LOG(LogTemp, Display, TEXT("Stack.Num(): %d"),temp);
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
