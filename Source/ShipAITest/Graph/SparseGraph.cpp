// Fill out your copyright notice in the Description page of Project Settings.

#include "SparseGraph.h"


//------------------------------ GetNode -------------------------------------
//
//  const and non const methods for obtaining a reference to a specific node
//----------------------------------------------------------------------------
const class NavGraphNode2D&  SparseGraph::GetNode(int32 Idx)const
{
	check((Idx < (int32)Nodes.Num()) &&
		(Idx >= 0) &&
		"SparseGraph::GetNode(): invalid index");

	return Nodes[Idx];
}

//non const version
NavGraphNode2D&  SparseGraph::GetNode(int32 Idx)
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
const NavGraphEdge& SparseGraph::GetEdge(int32 F, int32 T)const
{
	check((F < Nodes.Num()) &&
		(F >= 0) &&
		Nodes[F].GetIndex() != InvalidNodeIndex &&
		"SparseGraph::GetEdge(): invalid 'From' index");

	check((T < Nodes.Num()) &&
		(T >= 0) &&
		Nodes[T].GetIndex() != InvalidNodeIndex &&
		"SparseGraph::GetEdge(): invalid 'To' index");


	for (auto& Edge : Edges[F])
	{
		if (Edge.GetFrom() && Edge.GetTo() == T) return Edge;
	}

	check(0 && "<SparseGraph::GetEdge>: edge does not exist");
	return Edges[0][0]; // Should never run
}

//non const version
NavGraphEdge& SparseGraph::GetEdge(int32 F, int32 T)
{
	check((F < Nodes.Num()) &&
		(F >= 0) &&
		Nodes[F].GetIndex() != InvalidNodeIndex &&
		"SparseGraph::GetEdge(): invalid 'From' index");

	check((T < Nodes.Num()) &&
		(T >= 0) &&
		Nodes[T].GetIndex() != InvalidNodeIndex &&
		"SparseGraph::GetEdge(): invalid 'To' index");


	for (NavGraphEdge& Edge : Edges[F])
	{
		if (Edge.GetFrom() == F && Edge.GetTo() == T) return Edge;
	}

	check(0 && "<SparseGraph::GetEdge>: edge does not exist");
	return Edges[0][0]; // Should never run
}

//-------------------------- AddEdge ------------------------------------------
//
//  Use this to add an edge to the graph. The method will ensure that the
//  edge passed as a parameter is valid before adding it to the graph. If the
//  graph is a digraph then a similar edge connecting the nodes in the opposite
//  direction will be automatically added.
//-----------------------------------------------------------------------------

void SparseGraph::AddEdge(NavGraphEdge Edge)
{
	//first make sure the from and to nodes exist within the graph 
	check((Edge.GetFrom() < NextNodeIndex) && (Edge.GetTo() < NextNodeIndex) &&
		"SparseGraph::AddEdge(): invalid node index");

	//make sure both nodes are active before adding the edge
	if ((Nodes[Edge.GetTo()].GetIndex() != InvalidNodeIndex) &&
		(Nodes[Edge.GetFrom()].GetIndex() != InvalidNodeIndex))
	{
		//add the edge, first making sure it is unique
		if (UniqueEdge(Edge.GetFrom(), Edge.GetTo()))
		{
			Edges[Edge.GetFrom()].Emplace(Edge);
		}

		//if the graph is undirected we must add another connection in the opposite
		//direction

		//check to make sure the edge is unique before adding
		if (UniqueEdge(Edge.GetTo(), Edge.GetFrom()))
		{
			NavGraphEdge NewEdge = Edge;

			NewEdge.SetTo(Edge.GetFrom());
			NewEdge.SetFrom(Edge.GetTo());

			Edges[Edge.GetTo()].Emplace(NewEdge);
		}

	}
}

//-------------------------------- UniqueEdge ----------------------------
//
//  returns true if the edge is not present in the graph. Used when adding
//  edges to prevent duplication
//------------------------------------------------------------------------
bool SparseGraph::UniqueEdge(int32 F, int32 T)const
{
	for (auto Edge : Edges[F])
	{
		if (Edge.GetTo() == T)
		{
			return false;
		}
	}

	return true;
}
//-------------------------- AddNode -------------------------------------
//
//  Given a node this method first checks to see if the node has been added
//  previously but is now innactive. If it is, it is reactivated.
//
//  If the node has not been added previously, it is checked to make sure its
//  index matches the next node index before being added to the graph
//------------------------------------------------------------------------
int SparseGraph::AddNode(NavGraphNode2D  Node)
{
	if (Node.GetIndex() < Nodes.Num())
	{
		//make sure the client is not trying to add a node with the same ID as
		//a currently active node
		check(Nodes[Node.GetIndex()].GetIndex() == InvalidNodeIndex &&
			"<SparseGraph::AddNode>: Attempting to add a node with a duplicate ID");

		Nodes[Node.GetIndex()] = Node;

		return NextNodeIndex;
	}

	else
	{
		//make sure the new node has been indexed correctly
		check(Node.GetIndex() == NextNodeIndex && "<SparseGraph::AddNode>:invalid index");

		Nodes.Emplace(Node);
		Edges.Emplace(EdgeList());

		return NextNodeIndex++;
	}
}

//------------------------------- RemoveNode -----------------------------
//
//  Removes a node from the graph and removes any links to neighbouring
//  nodes
//------------------------------------------------------------------------
void SparseGraph::RemoveNode(int node)
{
	check(node < (int)Nodes.Num() && "<SparseGraph::RemoveNode>: invalid node index");

	//set this node's index to invalid_node_index
	Nodes[node].SetIndex(InvalidNodeIndex);

	//if the graph is not directed remove all edges leading to this node and then
	//clear the edges leading from the node
	for (auto& curEdge : Edges[node])
	{
		for (int32 curE = 0; curE < Edges[curEdge.GetTo()].Num() ; curE++)
		{
			if (Edges[curEdge.GetTo()][curE].GetTo() == node)
			{
				Edges[curEdge.GetTo()].RemoveAt(curE);

				break;
			}
		}
	}

	//finally, clear this node's edges
	Edges[node].Empty();
}