// Fill out your copyright notice in the Description page of Project Settings.

#include "GraphFunctions.h"
//-----------------------------------------------------------------------------
//
//  Name:   GraphFunctions.h
//
//  Author: Altered Mat Buckland (www.ai-junkie.com)
//
//  Desc:   As the name implies, some useful functions you can use with your
//          graphs. 

//          For the function templates, make sure your graph interface complies
//          with the SparseGraph class
//-----------------------------------------------------------------------------

//--------------------------- ValidNeighbour -----------------------------
//
//  returns true if x,y is a valid position in the map
//------------------------------------------------------------------------
bool ValidNeighbour(int32 X, int32 Y, int32 NumCellsX, int32 NumCellsY)
{
	return !((X < 0) || (X >= NumCellsX) || (Y < 0) || (Y >= NumCellsY));
}

//--------------------------- ValidNeighbour3D -----------------------------
//
//  returns true if x,y is a valid position in the map
//------------------------------------------------------------------------
bool ValidNeighbour3D(int32 X, int32 Y, int32 Z, int32 NumCellsX, int32 NumCellsY, int32 NumCellsZ)
{
	return !((X < 0) || (X >= NumCellsX) || (Y < 0) || (Y >= NumCellsY) || (Z < 0) || (Z >= NumCellsZ));
}

//------------ GraphHelper_AddAllNeighboursToGridNode ------------------
//
//  use to add the eight neighboring edges of a graph node that 
//  is positioned in a grid layout
//------------------------------------------------------------------------
void GraphHelper_AddAllNeighboursToGridNode(SparseGraph& Graph,
	int32         Row,
	int32         Col,
	int32         NumCellsX,
	int32         NumCellsY)
{
	for (int32 i = -1; i<2; ++i)
	{
		for (int32 j = -1; j<2; ++j)
		{
			int32 NodeX = Col + j;
			int32 NodeY = Row + i;

			//skip if equal to this node
			if ((i == 0) && (j == 0)) continue;

			//check to see if this is a valid neighbour
			if (ValidNeighbour(NodeX, NodeY, NumCellsX, NumCellsY))
			{

				NavGraphEdge NewEdge(Row*NumCellsX + Col, NodeY*NumCellsX + NodeX, 0);
				//this neighbour is okay so it can be added
				Graph.AddEdge(NewEdge);

				//if graph is not a diagraph then an edge needs to be added going
				//in the other direction

				NavGraphEdge ReturningEdge(NodeY*NumCellsX + NodeX,
					Row*NumCellsX + Col, 0);
				Graph.AddEdge(ReturningEdge);

			}
		}
	}
}

//------------ GraphHelper_AddAllNeighboursToGridNode3D ------------------
//
//  use to add the 26 neighboring edges of a graph node that 
//  is positioned in a grid layout
//------------------------------------------------------------------------
void GraphHelper_AddAllNeighboursToGridNode3D(SparseGraph3D& Graph,
	int32         Row,
	int32         Col,
	int32		  Dep,
	int32         NumCellsX,
	int32         NumCellsY,
	int32		  NumCellsZ)
{
	for (int32 k = -1; k < 2; ++k)
	{
		for (int32 i = -1; i < 2; ++i)
		{
			for (int32 j = -1; j < 2; ++j)
			{
				int32 NodeX = Col + j;
				int32 NodeY = Row + i;
				int32 NodeZ = Dep + k;

				//skip if equal to this node
				if ((i == 0) && (j == 0) && (k == 0)) continue;

				//check to see if this is a valid neighbour
				if (ValidNeighbour3D(NodeX, NodeY, NodeZ, NumCellsX, NumCellsY, NumCellsZ))
				{

					NavGraphEdge NewEdge((Row*NumCellsX + Col) + (NumCellsX * NumCellsY * Dep), 
						(NodeY*NumCellsX + NodeX) + (NumCellsX * NumCellsY * NodeZ), 0);
					//this neighbour is okay so it can be added
					Graph.AddEdge(NewEdge);

					//if graph is not a diagraph then an edge needs to be added going
					//in the other direction

					NavGraphEdge ReturningEdge((NodeY*NumCellsX + NodeX) + (NumCellsX * NumCellsY * NodeZ),
						(Row*NumCellsX + Col) + (NumCellsX * NumCellsY * Dep), 0);
					Graph.AddEdge(ReturningEdge);

				}
			}
		}
	}
}

//--------------------------- GraphHelper_CreateGrid --------------------------
//
//  creates a graph based on a grid layout. This function requires the 
//  dimensions of the environment and the number of cells required horizontally
//  and vertically 
//-----------------------------------------------------------------------------
void GraphHelper_CreateGrid(SparseGraph& Graph,
	int32 Height,
	int32 Width,
	int32 NumCellsY,
	int32 NumCellsX)
{
	// Temporaries to help calculate each node center
	float MidX = Width / 2.f;
	float MidY = Height / 2.f;

	// first create all the nodes
	for (int32 row = 0; row < NumCellsY; ++row)
	{
		for (int32 col = 0; col < NumCellsX; ++col)
		{
			Graph.AddNode(NavGraphNode2D(Graph.GetNextFreeNodeIndex(),
				FVector2DPlus(MidX + (col*Width),
					MidY + (row*Height))));
		}
	}

	// then calculate edges
	for (int32 row = 0; row < NumCellsY; ++row)
	{
		for (int32 col = 0; col < NumCellsX; ++col)
		{
			GraphHelper_AddAllNeighboursToGridNode(Graph, row, col, NumCellsX, NumCellsY);
		}
	}
}

//--------------------------- GraphHelper_CreateGrid3D --------------------------
//
//  creates a graph based on a grid layout in 3D. This function requires the 
//  dimensions of the environment and the number of cells required horizontally
//  and vertically and how many node layers deep
//-------------------------------------------------------------------------------
void GraphHelper_CreateGrid3D(SparseGraph3D& Graph,
	int32 Height,
	int32 Width,
	int32 Depth,
	int32 NumCellsY,
	int32 NumCellsX,
	int32 NumCellsZ)
{
	// Temporaries to help calculate each node center
	float MidX = Width / 2.f;
	float MidY = Height / 2.f;
	float MidZ = Depth / 2.f;

	// first create all the nodes
	for (int32 dep = 0; dep < NumCellsZ; dep++)
	{
		for (int32 row = 0; row < NumCellsY; ++row)
		{
			for (int32 col = 0; col < NumCellsX; ++col)
			{
				Graph.AddNode(NavGraphNode(Graph.GetNextFreeNodeIndex(),
					FVector(MidX + (col*Width), MidY + (row*Height), MidZ + (dep*Depth))));
			}
		}
	}

	// then calculate edges
	for (int32 dep = 0; dep < NumCellsZ; dep++)
	{
		for (int32 row = 0; row < NumCellsY; ++row)
		{
			for (int32 col = 0; col < NumCellsX; ++col)
			{
				GraphHelper_AddAllNeighboursToGridNode3D(Graph, row, col, dep, NumCellsX, NumCellsY, NumCellsZ);
			}
		}
	}
}