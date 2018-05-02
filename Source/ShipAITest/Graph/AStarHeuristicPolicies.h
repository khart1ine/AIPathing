// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SparseGraph.h"

/**
 * 
 */
class SHIPAITEST_API Heuristic_Euclid
{
public:

	Heuristic_Euclid() {}

	//calculate the straight line distance from node nd1 to node nd2
	static float Calculate(const SparseGraph& G, int32 nd1, int32 nd2)
	{
		
		return FVector2D::Distance(G.GetNode(nd1).GetLocation(), G.GetNode(nd2).GetLocation());
	}
};
