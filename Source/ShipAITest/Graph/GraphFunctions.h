// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "../Vector2DPlus.h"
//#include "GraphEdgeTypes.h"
#include "SparseGraph.h"
#include "SparseGraph3D.h"
//#include "CoreMinimal.h"

bool ValidNeighbour(int32 X, int32 Y, int32 NumCellsX, int32 NumCellsY);

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

bool ValidNeighbour3D(int32 X, int32 Y, int32 Z, int32 NumCellsX, int32 NumCellsY, int32 NumCells);

void GraphHelper_AddAllNeighboursToGridNode3D(SparseGraph3D& Graph,
	int32         Row,
	int32         Col,
	int32		  Depth,
	int32         NumCellsX,
	int32         NumCellsY,
	int32		  NumCellsZ);

void GraphHelper_CreateGrid3D(SparseGraph3D& Graph,
	int32 Height,
	int32 Width,
	int32 Depth,
	int32 NumCellsY,
	int32 NumCellsX,
	int32 NumCellsZ);