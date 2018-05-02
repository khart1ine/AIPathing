// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SparseGraph.h"
#include "GraphEdgeTypes.h"
#include "SparseGraph3D.h"

/**
 * 
 */
class SHIPAITEST_API Graph_SearchDFS
{
private:

	enum { visited, unvisited, no_parent_assigned };

	//a pointer to the graph to be searched
	const SparseGraph* Graph;
	const SparseGraph3D* Graph3D;

	//this records the indexes of all the nodes that are visited as the
	//search progresses
	TArray<int32> NodesVisited;

	//this holds the route taken to the target.Given a node index, the value
	//at that index is the node's parent. ie if the path to the target is
	//3-8-27, then Route[8] will hold 3 and Route[27] will hold 8.
	TArray<int32>  Route;

	//As the search progresses, this will hold all the edges the algorithm has
	//examined. THIS IS NOT NECESSARY FOR THE SEARCH, IT IS HERE PURELY
	//TO PROVIDE THE USER WITH SOME VISUAL FEEDBACK
	TArray<const NavGraphEdge*>  SpanningTree;

	//the source and target node indices
	int32 Source, Target;

	//true if a path to the target has been found
	bool PathFound;

	//this method performs the DFS search
	bool Search();
	bool Search3D();

public:

	Graph_SearchDFS(const SparseGraph* Gra, int32 Sou, int32 Tar = -1) :
		Graph(Gra), Source(Sou), Target(Tar), PathFound(false), Graph3D(nullptr)
	{
		NodesVisited.Init(unvisited, Graph->NumNodes());
		Route.Init(no_parent_assigned, Graph->NumNodes());
		PathFound = Search();
	}

	Graph_SearchDFS(const SparseGraph3D* Gra, int32 Sou, int32 Tar = -1) :
		Graph3D(Gra), Source(Sou), Target(Tar), PathFound(false), Graph(nullptr)
	{
		NodesVisited.Init(unvisited, Graph3D->NumNodes());
		Route.Init(no_parent_assigned, Graph3D->NumNodes());
		PathFound = Search3D();
	}

	//returns true if the target node has been located
	bool Found()const { return PathFound; }

	//returns a vector containing pointers to all the edges the search has examined
	TArray<const NavGraphEdge*> GetSearchTree()const { return SpanningTree; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	TArray<int32> GetPathToTarget()const;

};

//----------------------------- Graph_SearchBFS -------------------------------
//
//-----------------------------------------------------------------------------
class  SHIPAITEST_API  Graph_SearchBFS
{
private:

	//to aid legibility
	enum { visited, unvisited, no_parent_assigned };

	//create a typedef for the edge type used by the graph
	//typedef typename GraphEdgeTypes::EdgeType Edge;

private:

	//a reference to the graph to be searched
	const SparseGraph& Graph;

	//this records the indexes of all the nodes that are visited as the
	//search progresses
	TArray<int32> NodesVisited;

	//this holds the route taken to the target. Given a node index, the value
	//at that index is the node's parent. ie if the path to the target is
	//3-8-27, then m_Route[8] will hold 3 and m_Route[27] will hold 8.
	TArray<int32>  Route;

	//the source and target node indices
	int32 Source, Target;

	//true if a path to the target has been found
	bool PathFound;

	//As the search progresses, this will hold all the edges the algorithm has
	//examined. THIS IS NOT NECESSARY FOR THE SEARCH, IT IS HERE PURELY
	//TO PROVIDE THE USER WITH SOME VISUAL FEEDBACK
	TArray<const NavGraphEdge*>  SpanningTree;


	//the BFS algorithm is very similar to the DFS except that it uses a
	//FIFO queue instead of a stack.
	bool Search();


public:

	Graph_SearchBFS(const SparseGraph& Gra,
		int Sou,
		int Tar = -1) :Graph(Gra),
		Source(Sou),
		Target(Tar),
		PathFound(false)
	{
		NodesVisited.Init(unvisited, Graph.NumNodes());
		Route.Init(no_parent_assigned, Graph.NumNodes());
		PathFound = Search();
	}

	bool   Found()const { return PathFound; }

	//returns a vector containing pointers to all the edges the search has examined
	TArray<const NavGraphEdge*> GetSearchTree()const { return SpanningTree; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	TArray<int32> GetPathToTarget()const;
};

//----------------------- Graph_SearchDijkstra --------------------------------
//
//  Given a graph, source and optional target this class solves for
//  single source shortest paths (without a target being specified) or 
//  shortest path from source to target.
//
//  The algorithm used is a priority queue implementation of Dijkstra's.
//  note how similar this is to the algorithm used in Graph_MinSpanningTree.
//  The main difference is in the calculation of the priority in the line:
//  
//  double NewCost = m_CostToThisNode[best] + pE->Cost;
//------------------------------------------------------------------------
class  SHIPAITEST_API  Graph_SearchDijkstra
{
private:

	//create a typedef for the edge type used by the graph
	//typedef typename GraphEdgeTypes::EdgeType Edge;

private:

	//a reference to the graph to be searched
	const SparseGraph& Graph;

	//this TArray contains the edges that comprise the shortest path tree - 
	//a directed sub-tree of the graph taht encapsulates the best paths from
	//every node on the SPT to the source node
	TArray<const NavGraphEdge*> ShortestPathTree;

	//this is indexed into the node index and holds the total cost of the best
	//path found so far to the given node.  For example, CostToThisNode[5]
	//will hold the total cost of all the edges that comprise the best path
	//to node 5 found so far in the search(if node 5 is present and has been visited).
	TArray<float>  CostToThisNode;
	//this is an indexed (by node) vector of 'parent' edges leading to nodes 
	//connected to the SPT but that have not been added to the SPT yet. This is
	//a little like the stack or queue used in BST and DST searches.
	TArray<const NavGraphEdge*>     SearchFrontier;


	///path found so far to the given node. For example, CostToHidethisNode[5]

	//the source and target node indices
	int32 Source, Target;


	//the BFS algorithm is very similar to the DFS except that it uses a
	//FIFO queue instead of a stack.
	void Search();


public:

	Graph_SearchDijkstra(const SparseGraph& Gra,
		int Sou,
		int Tar = -1) :	Graph(Gra),
						Source(Sou),
						Target(Tar)
	{
		ShortestPathTree.Init(nullptr, Graph.NumNodes());
		CostToThisNode.Init(0.0f, Graph.NumNodes());
		SearchFrontier.Init(nullptr, Graph.NumNodes());
		Search();
	}


	//returns the vector of edges that defines the SPT. If a target was given
	//in the constructor then this will be an SPT comprising of all the nodes
	//examined before the target was found, else it will contain all the nodes
	TArray<const NavGraphEdge*> GetSearchSPT()const { return ShortestPathTree; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	TArray<int32> GetPathToTarget()const;

	//returns the total cost to the target
	float GetCostToTarget()const { return CostToThisNode[Target]; }

	//returns the total cost to the given node
	float GetCostToNode(unsigned int nd)const { return CostToThisNode[nd]; }
};

//------------------------------- Graph_SearchAStar --------------------------
//
//  this searchs a graph using the distance between the target node and the 
//  currently considered node as a heuristic.
//
//  This search is more commonly known as A* 
//-----------------------------------------------------------------------------
class Graph_SearchAStar
{
private:

	//create a typedef for the edge type used by the graph
	//	typedef typename graph_type::EdgeType Edge;

private:

	const SparseGraph& Graph;


	//indexed into my node. Contains the 'real' accumulative cost to that node
	TArray<float> GCosts;

	//indexed into by node. Contains the cost from adding m_GCosts[n] to
	//the heuristic cost from n to the target node. This is the vector the
	//iPQ indexes into.
	TArray<float> FCosts;

	TArray<const NavGraphEdge*> ShortestPathTree;
	TArray<const NavGraphEdge*> SearchFrontier;

	int32	Source, Target;

	//the A* search algorithm
	void Search();

public:

	Graph_SearchAStar(const SparseGraph& Gra,
		int   Sou,
		int   Tar) :Graph(Gra),
		Source(Sou),
		Target(Tar)
	{
		ShortestPathTree.Init(nullptr, Graph.NumNodes()),
		SearchFrontier.Init(nullptr, Graph.NumNodes()),
		GCosts.Init(0.0f, Graph.NumNodes()),
		FCosts.Init(0.0f, Graph.NumNodes()),

		Search();
	}

	//returns the vector of edges that the algorithm has examined
	TArray<const NavGraphEdge*> GetSPT()const { return ShortestPathTree; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	TArray<int32> GetPathToTarget()const;

	//returns the total cost to the target
	float GetCostToTarget()const { return GCosts[Target]; }
};

