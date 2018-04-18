// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SparseGraph.h"

/**
 * 
 */
class SHIPAITEST_API Graph_SearchDFS
{
private:

	enum { no_parent_assigned = -1 , visited, unvisited};

	//a reference to the graph to be searched
	const SparseGraph& Graph;

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

public:

	Graph_SearchDFS(const SparseGraph& Gra, int32 Sou, int32 Tar = -1) :
		Graph(Gra), Source(Sou), Target(Tar), PathFound(false)
	{
		NodesVisited.Init(unvisited, Graph.NumNodes());
		Route.Init(no_parent_assigned, Graph.NumNodes());
		PathFound = Search();
	}

	//returns true if the target node has been located
	bool Found()const { return PathFound; }

	//returns a vector containing pointers to all the edges the search has examined
	TArray<const NavGraphEdge*> GetSearchTree()const { return SpanningTree; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	TArray<int32> GetPathToTarget()const;

	
};


