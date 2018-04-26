// Fill out your copyright notice in the Description page of Project Settings.

#include "Pathfinder2D.h"
#include "GraphFunctions.h"

//----------------------- CreateGraph ------------------------------------
//
//------------------------------------------------------------------------
void Pathfinder2D::CreateGraph(int32 CellsUp, int32 CellsAcross, int32 CellSize)
{
	TerrainType.Init(normal, CellsUp * CellsAcross);

	CellsX = CellsAcross;
	CellsY = CellsUp;
	Width = CellSize;
	Height = CellSize;

	// delete any old graph
	delete PGraph;

	// create the graph
	PGraph = new SparseGraph();

	GraphHelper_CreateGrid(*PGraph, Height, Width, CellsUp, CellsAcross);

	// initialize source and target indices to mid-top and mid-bottom of grid 
	//SourceCell = (CellsAcross / 2);
	SourceCell = CellsAcross * (CellsUp - 1);
	IsStartAdded = true;
	//TargetCell = CellsUp * CellsAcross - SourceCell - 1;
	TargetCell = (CellsAcross -1);
	IsFinishAdded = true;

	//UpdateGraphFromBrush(obstacle, CellsAcross + 1);
	//UpdateGraphFromBrush(obstacle, 21);
	//UpdateGraphFromBrush(obstacle, 16);
	UpdateGraphFromBrush(obstacle, 12);
	//UpdateGraphFromBrush(obstacle, 6);
	//UpdateGraphFromBrush(obstacle, CellsAcross * (CellsUp - 1) - 2);


	Path.Empty();
	SubTree.Empty();

	CurrentAlgorithm = search_dfs;
}

//--------------------------- UpdateAlgorithm ---------------------------------
void Pathfinder2D::UpdateAlgorithm()
{
	switch (CurrentAlgorithm)
	{
	case non:
		break;
	case search_dfs:
		CreatePathDFS(); break;
	case search_bfs:
		break;
	case search_dijkstra:
		break;
	case search_astar:
		break;
	}
}

//------------------------- CreatePathDFS --------------------------------
//
//  uses DFS to find a path between the start and target cells.
//  Stores the path as a series of node indexes in Path.
//------------------------------------------------------------------------
void Pathfinder2D::CreatePathDFS()
{
	CurrentAlgorithm = search_dfs;

	Path.Empty();
	SubTree.Empty();

	Graph_SearchDFS DFS(PGraph, SourceCell, TargetCell);

	if (DFS.Found())
	{
		Path = DFS.GetPathToTarget();
	}

	SubTree = DFS.GetSearchTree();

	CostToTarget = 0.0f;
}
//------------------------- CreatePathBFS --------------------------------
//
//  uses BFS to find a path between the start and target cells.
//  Stores the path as a series of node indexes in m_Path.
//------------------------------------------------------------------------
void Pathfinder2D::CreatePathBFS()
{
	//set current algorithm
	CurrentAlgorithm = search_bfs;

	//clear any existing path
	Path.Empty();
	SubTree.Empty();

	//do the search
	Graph_SearchBFS BFS(*PGraph, SourceCell, TargetCell);


	//now grab the path (if one has been found)
	if (BFS.Found())
	{
		Path = BFS.GetPathToTarget();
	}

	SubTree = BFS.GetSearchTree();

	CostToTarget = 0.0;
}

//-------------------------- CreatePathDijkstra --------------------------
//
//  creates a path from m_iSourceCell to m_iTargetCell using Dijkstra's algorithm
//------------------------------------------------------------------------
void Pathfinder2D::CreatePathDijkstra()
{
	//set current algorithm
	CurrentAlgorithm = search_dijkstra;

	Graph_SearchDijkstra Djk(*PGraph, SourceCell, TargetCell);

	Path = Djk.GetPathToTarget();

	SubTree = Djk.GetSearchSPT();

	CostToTarget = Djk.GetCostToTarget();
}


void Pathfinder2D::UpdateGraphFromBrush(int32 brush, int32 CellIndex)
{
	if (brush == 1)
	{
		PGraph->RemoveNode(CellIndex);
	}
}