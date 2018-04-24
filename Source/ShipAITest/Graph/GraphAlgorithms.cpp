// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphAlgorithms.h"
#include "Engine.h"
#include "../DataStructure/PriorityQueue.h"

bool Graph_SearchDFS::Search()
{
	//create a std stack of edges
	TArray<const NavGraphEdge*> Stack;

	//create a dummy edge and put on the stack
	NavGraphEdge Dummy(Source, Source, 0);

	//const NavGraphEdge* Next = nullptr;
	const NavGraphEdge* pE = nullptr;

	Stack.Emplace(&Dummy);

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
				Stack.Emplace(pE);
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

//----------------------------- Graph_SearchBFS -------------------------------
//
//-----------------------------------------------------------------------------

bool Graph_SearchBFS::Search()
{
	//create a std queue of edges
	TQueue<const NavGraphEdge*> Q;

	const NavGraphEdge Dummy(Source, Source, 0);

	//create a dummy edge and put on the queue
	Q.Enqueue(&Dummy);

	//mark the source node as visited
	NodesVisited[Source] = visited;

	//while there are edges in the queue keep searching
	while (!Q.IsEmpty())
	{
		//grab the next edge
		const NavGraphEdge* Next = nullptr;
		Q.Peek(Next);

		Q.Pop();

		//mark the parent of this node
		Route[Next->GetTo()] = Next->GetFrom();

		//put it on the tree. (making sure the dummy edge is not placed on the tree)
		if (Next != &Dummy)
		{
			SpanningTree.Emplace(Next);
		}

		//exit if the target has been found
		if (Next->GetTo() == Target)
		{
			return true;
		}

		//push the edges leading from the node at the end of this edge 
		//onto the queue
		SparseGraph::ConstEdgeIterator ConstEdgeItr(Graph, Next->GetTo());

		for (const NavGraphEdge* pE = ConstEdgeItr.begin();
			!ConstEdgeItr.end();
			pE = ConstEdgeItr.next())
		{
			//if the node hasn't already been visited we can push the
			//edge onto the queue
			if (NodesVisited[pE->GetTo()] == unvisited)
			{
				Q.Enqueue(pE);

				//and mark it visited
				NodesVisited[pE->GetTo()] = visited;
			}
		}
	}

	//no path to target
	return false;
}


//-----------------------------------------------------------------------------
TArray<int32> Graph_SearchBFS::GetPathToTarget()const
{
	TArray<int32> path;

	//just return an empty path if no path to target found or if
	//no target has been specified
	if (!Found() || Target<0) return path;

	int nd = Target;

	path.Insert(nd, 0);

	while (nd != Source)
	{
		nd = Route[nd];

		path.Insert(nd, 0);
	}

	return path;
}

void Graph_SearchDijkstra::Search()
{
	//create an indexed priority queue that sorts smallest to largest
	//(front to back).Note that the maximum number of elements the iPQ
	//may contain is N. This is because no node can be represented on the 
	//queue more than once.
	IndexedPriorityQLow<float> Pq(CostToThisNode, Graph.NumNodes());

	//put the source node on the queue
	Pq.Insert(Source);

	//while the queue is not empty
	while (!Pq.IsEmpty())
	{
		//get lowest cost node from the queue. Don't forget, the return value
		//is a *node index*, not the node itself. This node is the node not already
		//on the SPT that is the closest to the source node
		int NextClosestNode = Pq.Pop();

		//move this edge from the frontier to the shortest path tree
		ShortestPathTree[NextClosestNode] = SearchFrontier[NextClosestNode];

		//if the target has been found exit
		if (NextClosestNode == Target) return;

		//now to relax the edges.
		SparseGraph::ConstEdgeIterator ConstEdgeItr(Graph, NextClosestNode);

		//for each edge connected to the next closest node
		for (const NavGraphEdge* pE = ConstEdgeItr.begin();
			!ConstEdgeItr.end();
			pE = ConstEdgeItr.next())
		{
			//the total cost to the node this edge points to is the cost to the
			//current node plus the cost of the edge connecting them.
			float NewCost = CostToThisNode[NextClosestNode] + pE->GetCost();

			//if this edge has never been on the frontier make a note of the cost
			//to get to the node it points to, then add the edge to the frontier
			//and the destination node to the PQ.
			if (SearchFrontier[pE->GetTo()] == 0)
			{
				CostToThisNode[pE->GetTo()] = NewCost;

				Pq.Insert(pE->GetTo());

				SearchFrontier[pE->GetTo()] = pE;
			}

			//else test to see if the cost to reach the destination node via the
			//current node is cheaper than the cheapest cost found so far. If
			//this path is cheaper, we assign the new cost to the destination
			//node, update its entry in the PQ to reflect the change and add the
			//edge to the frontier
			else if ((NewCost < CostToThisNode[pE->GetTo()]) &&
				(ShortestPathTree[pE->GetTo()] == 0))
			{
				CostToThisNode[pE->GetTo()] = NewCost;

				//because the cost is less than it was previously, the PQ must be
				//re-sorted to account for this.
				Pq.ChangePriority(pE->GetTo());

				SearchFrontier[pE->GetTo()] = pE;
			}
		}
	}
}

//-----------------------------------------------------------------------------
TArray<int> Graph_SearchDijkstra::GetPathToTarget()const
{
	TArray<int> Path;

	//just return an empty path if no target or no path found
	if (Target < 0)  return Path;

	int nd = Target;

	Path.Emplace(nd);

	while ((nd != Source) && (ShortestPathTree[nd] != 0))
	{
		nd = ShortestPathTree[nd]->GetFrom();

		Path.Emplace(nd);
	}

	return Path;
}