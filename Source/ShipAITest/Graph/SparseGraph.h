// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NodeTypeEnumerations.h"
#include "GraphNodeTypes.h"
#include "GraphEdgeTypes.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SHIPAITEST_API SparseGraph
{
public:
	typedef TArray<class NavGraphNode2D> NodeArray;
	typedef TArray <class NavGraphEdge> EdgeArray;
	SparseGraph(): NextNodeIndex(0){}
	
	~SparseGraph(){}

	/** returns the node at the given index **/
	const NavGraphNode2D& GetNode(int Idx)const;

	/** non const version **/
	NavGraphNode2D& GetNode(int Idx);

	/** returns the edge at the given index **/
	const NavGraphEdge& GetEdge(int F, int T)const;

	/** non const version **/
	NavGraphEdge& GetEdge(int F, int T);

	/*

	//adds a node to the graph and returns its index
	int   AddNode(node_type node);

	//retrieves the next free node index
	int   GetNextFreeNodeIndex()const{return m_iNextNodeIndex;}

	//Use this to add an edge to the graph. The method will ensure that the
	//edge passed as a parameter is valid before adding it to the graph. If the
	//graph is a digraph then a similar edge connecting the nodes in the opposite
	//direction will be automatically added.
	void  AddEdge(EdgeType edge);

	*/


private:
	/** The nodes that comprise this graph **/
	NodeArray Nodes;

	/** An array of adjacent edges.  Each node index keys 
		into the list of edges associated with that node**/
	EdgeArray Edges;

	/** The index of the next node to be added **/
	int32 NextNodeIndex;
};
//------------------------------ GetNode -------------------------------------
//
//  const and non const methods for obtaining a reference to a specific node
//----------------------------------------------------------------------------
const class NavGraphNode2D&  SparseGraph::GetNode(int Idx)const
{
	check((Idx < (int32) Nodes.Num()) &&
		(Idx >= 0) &&
		"SparseGraph::GetNode(): invalid index");

	return Nodes[Idx];
}

//non const version
NavGraphNode2D&  SparseGraph::GetNode(int Idx)
{
	check((Idx < (int32)Nodes.Num()) &&
		(Idx >= 0) &&
		"<SparseGraph::GetNode>: invalid index");

	return Nodes[Idx];
}

//------------------------------ GetEdge -------------------------------------
//
//  const and non const methods for obtaining a reference to a specific edge
//----------------------------------------------------------------------------
const NavGraphEdge& SparseGraph::GetEdge(int F, int T)const
{
	check((F < Nodes.Num()) &&
		(F >= 0) &&
		Nodes[F].GetIndex() != InvalidNodeIndex &&
		"SparseGraph::GetEdge(): invalid 'From' index");

	check((T < Nodes.Num()) &&
		(T >= 0) &&
		Nodes[T].GetIndex() != InvalidNodeIndex &&
		"SparseGraph::GetEdge(): invalid 'To' index");


	for (auto& Edge : Edges)
	{
		if (Edge.GetTo() == T) return Edge;
	}

	check(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

//non const version
NavGraphEdge& SparseGraph::GetEdge(int F, int T)
{
	check((F < Nodes.Num()) &&
		(F >= 0) &&
		Nodes[F].GetIndex() != InvalidNodeIndex &&
		"SparseGraph::GetEdge(): invalid 'From' index");

	check((T < Nodes.Num()) &&
		(T >= 0) &&
		Nodes[T].GetIndex() != InvalidNodeIndex &&
		"SparseGraph::GetEdge(): invalid 'To' index");


	for (NavGraphEdge& Edge : Edges)
	{
		if (Edge.GetTo() == T) return Edge;
	}

	check(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

