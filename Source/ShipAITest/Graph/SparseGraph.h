// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "NodeTypeEnumerations.h"
#include "GraphNodeTypes.h"
#include "GraphEdgeTypes.h"
#include "CoreMinimal.h"

/**
 * 
 */

typedef TArray<NavGraphNode2D> NodeArray;
typedef TArray<NavGraphEdge> EdgeList;
typedef TArray<EdgeList> EdgeArray;

class SHIPAITEST_API SparseGraph
{
public:
	SparseGraph(): NextNodeIndex(0){}
	
	~SparseGraph(){}

	/** returns the node at the given index **/
	const NavGraphNode2D& GetNode(int32 Idx)const;

	/** non const version **/
	NavGraphNode2D& GetNode(int32 Idx);

	/** returns the edge at the given index **/
	const NavGraphEdge& GetEdge(int32 F, int32 T)const;

	/** non const version **/
	NavGraphEdge& GetEdge(int32 F, int32 T);

	//Use this to add an edge to the graph. The method will ensure that the
	//edge passed as a parameter is valid before adding it to the graph. If the
	//graph is a digraph then a similar edge connecting the nodes in the opposite
	//direction will be automatically added.
	void  AddEdge(NavGraphEdge Edge);

	//adds a node to the graph and returns its index
	int32   AddNode(NavGraphNode2D  Node);

	//retrieves the next free node index
	int32   GetNextFreeNodeIndex()const{return NextNodeIndex;}

	int NumNodes()const { return Nodes.Num(); }


private:
	/** The nodes that comprise this graph **/
	NodeArray Nodes;

	/** An array of adjacent edges.  (each node index keys into the 
    list of edges associated with that node)**/
	EdgeArray Edges;

	/** The index of the next node to be added **/
	int32 NextNodeIndex;

	//returns true if an edge is not already present in the graph. Used
	//when adding edges to make sure no duplicates are created.
	bool  UniqueEdge(int32 F, int32 T)const;

public:

	class ConstEdgeIterator
	{
	private:

		const SparseGraph & G;
		EdgeList CurEdge;
		const int32 NodeIndex;
		int32 IteratorIndex;

	public:
		
		ConstEdgeIterator(const SparseGraph& graph, int32 node) : 
			G(graph), NodeIndex(node), IteratorIndex(0)
		{
			CurEdge = G.Edges[NodeIndex];
		}

		const NavGraphEdge* begin()
		{
			IteratorIndex = 0;
			return &CurEdge[0];
		}

		const NavGraphEdge* next()
		{
			IteratorIndex++;

			if (IteratorIndex >= G.Edges[NodeIndex].Num())
			{
				return nullptr;
			}

			return &CurEdge[IteratorIndex];
		}

		bool end()
		{
			return (IteratorIndex == G.Edges[NodeIndex].Num());
		}
	};

	friend class ConstEdgeIterator;
};