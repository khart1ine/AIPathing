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

	enum { visited, unvisited, no_parent_assigned };

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

	int32 Source, Target;

	bool PathFound;

	bool Search();

public:

	Graph_SearchDFS(const SparseGraph& Gra, int32 Sou, int32 Tar = -1) :
		Graph(Gra), Source(Sou), Target(Tar), PathFound(false)
	{
		NodesVisited.Init(unvisited, Graph.NumNodes());
		Route.Init(no_parent_assigned, Graph.NumNodes());
		PathFound = Search();
	}

	bool Found()const { return PathFound; }

	TArray<const NavGraphEdge*> GetSearchTree()const { return SpanningTree; }

	TArray<int32> GetPathToTarget()const;
};


