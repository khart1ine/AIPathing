// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "../Vector2DPlus.h"
//#include "GraphEdgeTypes.h"
#include "SparseGraph.h"
//#include "CoreMinimal.h"

bool ValidNeighbour(int X, int Y, int NumCellsX, int NumCellsY);

void GraphHelper_AddAllNeighboursToGridNode(SparseGraph& Graph,
	int32         Row,
	int32         Col,
	int32         NumCellsX,
	int32         NumCellsY);

void GraphHelper_CreateGrid(SparseGraph& Graph,
	int32 Height,
	int32 Width,
	int32 NumCellsY,
	int32 NumCellsX);