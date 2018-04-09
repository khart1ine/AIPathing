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
bool ValidNeighbour(int X, int Y, int NumCellsX, int NumCellsY)
{
	return !((X < 0) || (X >= NumCellsX) || (Y < 0) || (Y >= NumCellsY));
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
				//calculate the distance to this node
				//	FVector2DPlus PosNode = Graph.GetNode(Row*NumCellsX + Col).GetLocation();
				//	FVector2DPlus PosNeighbour = Graph.GetNode(NodeY*NumCellsX + NodeX).GetLocation();

				//	float Dist = PosNode.Distance(PosNeighbour);

				NavGraphEdge NewEdge(Row*NumCellsX + Col, NodeY*NumCellsX + NodeX);
				//this neighbour is okay so it can be added
				//	graph_type::EdgeType NewEdge(row*NumCellsX + col,
				//	nodeY*NumCellsX + nodeX,
				//dist);
				Graph.AddEdge(NewEdge);

				//if graph is not a diagraph then an edge needs to be added going
				//in the other direction

				NavGraphEdge ReturningEdge(NodeY*NumCellsX + NodeX,
					Row*NumCellsX + Col);
				Graph.AddEdge(ReturningEdge);

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
	for (int row = 0; row < NumCellsY; ++row)
	{
		for (int col = 0; col < NumCellsX; ++col)
		{
			Graph.AddNode(NavGraphNode2D(Graph.GetNextFreeNodeIndex(),
				FVector2DPlus(MidX + (col*Width),
					MidY + (row*Height))));
		}
	}

	// then calculate edges
	for (int row = 0; row < NumCellsY; ++row)
	{
		for (int col = 0; col < NumCellsX; ++col)
		{
			GraphHelper_AddAllNeighboursToGridNode(Graph, row, col, NumCellsX, NumCellsY);
		}
	}
}