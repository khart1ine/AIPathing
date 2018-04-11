// Fill out your copyright notice in the Description page of Project Settings.

#include "Pathfinder2D.h"
#include "GraphFunctions.h"

void Pathfinder2D::CreateGraph(int32 CellsUp, int32 CellsAcross, int32 CellSize)
{
	TerrainType.Init(normal, CellsUp * CellsAcross);

	CellsX = CellsAcross;
	CellsY = CellsUp;
	Width = CellSize;
	Height = CellSize;

	delete PGraph;

	PGraph = new SparseGraph();

	GraphHelper_CreateGrid(*PGraph, Height, Width, CellsUp, CellsAcross);

	// initialize source and target indices to mid-top and mid-bottom of grid 
	SourceCell = (CellsAcross / 2);
	IsStartAdded = true;
	TargetCell = CellsUp * CellsAcross - SourceCell - 1;
	IsFinishAdded = true;

	Path.Empty();
	SubTree.Empty();

	CurrentAlgorithm = search_dfs;
}

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

void Pathfinder2D::CreatePathDFS()
{
	CurrentAlgorithm = search_dfs;

	Path.Empty();
	SubTree.Empty();

	Graph_SearchDFS DFS(*PGraph, SourceCell, TargetCell);

	if (DFS.Found())
	{
		Path = DFS.GetPathToTarget();
	}

	SubTree = DFS.GetSearchTree();

	CostToTarget = 0.0f;
}

void Pathfinder2D::UpdateGraphFromBrush(int32 brush, int32 CellIndex)
{

}