// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Vector2DPlus.h"
#include "NodeTypeEnumerations.h"
#include "CoreMinimal.h"

//-----------------------------------------------------------------------------
//
//  Name:   GraphNodeTypes.h
//
//  Author: Altered Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Node classes to be used with graphs
//-----------------------------------------------------------------------------

class SHIPAITEST_API GraphNode
{
public:

	GraphNode():Index(InvalidNodeIndex){}
	GraphNode(int Ind):Index(Ind){}

	virtual ~GraphNode(){};

	int32 GetIndex()const { return Index;}
	void SetIndex(int32 NewIndex){Index = NewIndex; }


protected:
	/** Every node has a unique index.  Valid index >= 0 **/
	int32 Index;
};

class NavGraphNode2D : public GraphNode
{

public:
	//constructor
	NavGraphNode2D(int Idx, FVector2DPlus Loc): GraphNode(Idx),
												Location(Loc)
	{}

	virtual ~NavGraphNode2D(){}

	FVector2DPlus GetLocation()const { return Location; }
	void SetLocation(FVector2DPlus NewLocation) { Location = NewLocation; }

protected:
	/** the nodes position **/
	FVector2DPlus Location;

};

class NavGraphNode : public GraphNode
{

public:
	//constructor
	NavGraphNode(int Idx, FVector Loc) : GraphNode(Idx),
		Location(Loc)
	{}

	virtual ~NavGraphNode() {}

	FVector GetLocation()const { return Location; }
	void SetLocation(FVector NewLocation) { Location = NewLocation; }

protected:
	/** the nodes position **/
	FVector Location;

};
