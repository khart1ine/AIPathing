// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NodeTypeEnumerations.h"
#include "CoreMinimal.h"

//-----------------------------------------------------------------------------
//
//  Name:   GraphEdgeTypes.h
//
//  Author: Modified Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Class to define an edge connecting two nodes.
//          
//          An edge has an associated cost.
//-----------------------------------------------------------------------------
class SHIPAITEST_API GraphEdge
{
public:
	GraphEdge(int32 F, int32 T):From(F), To(T){}
	GraphEdge():From(InvalidNodeIndex), To(InvalidNodeIndex) {}
	virtual ~GraphEdge(){}

	int32 GetFrom()const { return From; }
	void SetFrom(int32 NewIndex) { From = NewIndex; }

	int32 GetTo()const { return To; }
	void SetTo(int32 NewIndex) { To = NewIndex; }

	bool operator ==(const GraphEdge& Rhs)
	{
		return Rhs.From == this->From &&
				Rhs.To == this->To;
	}

	bool operator !=(const GraphEdge& Rhs)
	{
		return !(*this == Rhs);
	}


protected:
	/** Edge connects two nodes.  Valid nodes are always >=0 **/
	int32 From;
	int32 To;
};

class NavGraphEdge : public GraphEdge
{
public:
	NavGraphEdge (	int32 F,
					int32 T,
					int32 Fl = 0):GraphEdge(F,T), Flags(Fl)
	{}

	int GetFlags()const { return Flags; }
	void SetFlags(int32 F) { Flags = F; }

protected:
	int32 Flags;
};
