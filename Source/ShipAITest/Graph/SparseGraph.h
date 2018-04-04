// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NodeTypeEnumerations.h"
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
	~SparseGraph();

	/** returns the node at the given index **/
	const NavGraphNode2D& GetNode(int Idx)const;

	/** non const version **/
	NavGraphNode2D& GetNode(int Idx);

	/** returns the edge at the given index **/
	const NavGraphEdge& GetNode(int F, int T)const;

	/** non const version **/
	NavGraphEdge& GetNode(int F, int T);




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
	check((Idx < (int Nodes.Num()) &&
		(Idx >= 0) &&
		"SparseGraph::GetNode(): invalid index");

	return Nodes[Idx];
}

//non const version
NavGraphNode2D&  SparseGraph::GetNode(int idx)
{
	check((idx < (int)Nodes.Num()) &&
		(idx >= 0) &&
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


	for (NavGraphEdge& Edge : Edges)
	{
		if Edge->GetTo() == To) return *Edge;
	}

	check(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

//non const version
template <class node_type, class edge_type>
edge_type& SparseGraph<node_type, edge_type>::GetEdge(int from, int to)
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
		if Edge->GetTo() == To) return *Edge;
	}

	check(0 && "<SparseGraph::GetEdge>: edge does not exist");
}

