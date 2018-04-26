#include "Pathfinder.h"
#include "GraphFunctions.h"

//----------------------- CreateGraph ------------------------------------
//
//------------------------------------------------------------------------
void Pathfinder::CreateGraph(int32 CellsUp, int32 CellsAcross, int32 CellsDeep, int32 CellSize)
{
	TerrainType.Init(normal, CellsUp * CellsAcross * CellsDeep);

	CellsX = CellsAcross;
	CellsY = CellsUp;
	CellsZ = CellsDeep;
	Width = CellSize;
	Height = CellSize;
	Depth = CellSize;

	// delete any old graph
	delete PGraph;

	// create the graph
	PGraph = new SparseGraph3D();

	GraphHelper_CreateGrid3D(*PGraph, Height, Width, Depth, CellsUp, CellsAcross, CellsDeep);

	// initialize source and target indices 
	SourceCell = 0;
	IsStartAdded = true;
	TargetCell = (CellsAcross * CellsDeep * CellsUp) - 1;
	IsFinishAdded = true;

	UpdateGraphFromBrush(obstacle, 31);
	UpdateGraphFromBrush(obstacle, 30);

	Path.Empty();
	SubTree.Empty();

	CurrentAlgorithm = search_dfs;
}

//--------------------------- UpdateAlgorithm ---------------------------------
void Pathfinder::UpdateAlgorithm()
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
void Pathfinder::CreatePathDFS()
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

void Pathfinder::UpdateGraphFromBrush(int32 brush, int32 CellIndex)
{
	if (brush == 1)
	{
		PGraph->RemoveNode(CellIndex);
	}
}