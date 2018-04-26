// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphEdgeTypes.h"
#include "GraphNodeTypes.h"
#include "GraphAlgorithms.h"
#include "Graph/Pathfinder2D.h"
#include "Graph/SparseGraph3D.h"

/**
*
*/
class SHIPAITEST_API Pathfinder
{

public:

	enum EBrushType
	{
		normal = 0,
		obstacle = 1,
		water = 2,
		mud = 3,
		source = 4,
		target = 5

	};

	enum EAlgorithmType
	{
		non,
		search_astar,
		search_bfs,
		search_dfs,
		search_dijkstra
	};

private:
	
	// terrain type of each cell
	TArray<int32> TerrainType;

	SparseGraph3D* PGraph;

	// this is used to store any subtree returned from graph algorithms
	TArray<const NavGraphEdge*> SubTree;

	// total cost of the path fro target to source
	float CostToTarget;

	// currently selected algorithm
	EAlgorithmType CurrentAlgorithm;
	// current terrain brush
	EBrushType CurrentTerrainBrush;

	// dimensions of the cell
	float Width;
	float Height;
	float Depth;

	// number of ceslls horizontally and vertically
	int32 CellsX;
	int32 CellsY;
	int32 CellsZ;

	// indices of the source and target cell
	int32 SourceCell;
	int32 TargetCell;

	// flags to indiate if the start and finish points have been added
	bool IsStartAdded;
	bool IsFinishAdded;

	// calls the appropriate search algorithm
	void UpdateAlgorithm();

	// Helper funciton for PaintTerrain , unused
	void UpdateGraphFromBrush(int32 brush, int32 CellIndex);

public:
	Pathfinder() : IsStartAdded(false), IsFinishAdded(false), Width(0),
				   Height(0), Depth(0), CellsX(0), CellsY(0), CellsZ(0), CurrentTerrainBrush(normal),
				   SourceCell(0), TargetCell(0), CostToTarget(0.0f), PGraph(nullptr)
	{}

	~Pathfinder() { delete PGraph; }


	void CreateGraph(int32 CellsUp, int32 CellsAcross, int32 CellsDeep, int32 CellSize);

	// pathing algorithms
	void CreatePathDFS();

	TArray<int32> Path;

	TArray<int32> GetPath() { return Path; }
	TArray<const NavGraphEdge*> GetSubTree() { return SubTree; }
};
