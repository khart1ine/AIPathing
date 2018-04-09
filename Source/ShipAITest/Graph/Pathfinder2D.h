// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SparseGraph.h"
#include "GraphEdgeTypes.h"
#include "GraphNodeTypes.h"
#include "GraphAlgorithms.h"

/**
 * 
 */
class SHIPAITEST_API Pathfinder2D
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

	TArray<int32> TerrainType;
	TArray<int32> Path;

	SparseGraph* PGraph;

	TArray<const NavGraphEdge*> SubTree;

	float CostToTarget;

	EAlgorithmType CurrentAlgorithm;
	EBrushType CurrentTerrainBrush;

	float Width;
	float Height;

	int32 CellsX;
	int32 CellsY;

	int32 SourceCell;
	int32 TargetCell;

	bool IsStartAdded;
	bool IsFinishAdded;
	
	void UpdateAlgorithm();

	void UpdateGraphFromBrush(int32 brush, int32 CellIndex);

public:

	Pathfinder2D():IsStartAdded(false), IsFinishAdded(false), Width(0),
				   Height(0), CellsX(0), CellsY(0), CurrentTerrainBrush(normal),
				   SourceCell(0), TargetCell(0), CostToTarget(0.0f), PGraph(nullptr)
	{}

	~Pathfinder2D() { delete PGraph; }

	void CreateGraph(int32 CellsUp, int32 CellsAcross, int32 CellSize);

	void CreatePathDFS();

};
