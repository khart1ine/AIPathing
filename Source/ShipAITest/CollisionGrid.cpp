// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionGrid.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"

ACollisionGrid::ACollisionGrid()
{

	Grid = FVector(0.0f, 0.0f, 0.0f);
	GridOffset = 100.f;
	GridLimit = 5.f;
}

void ACollisionGrid::BeginPlay()
{
	Super::BeginPlay();
	Grid = GetTransform().GetLocation();

	float CollisionDistance = 0.f;

	for (int i = 0; i < GridLimit; i++)
	{
		for (int j = 0; j < GridLimit; j++)
		{
			for (int k = 0; k < GridLimit; k++)
			{
				DrawDebugBox(GetWorld(), Grid + FVector(i * GridOffset, j * GridOffset, k * GridOffset), 
					FVector(GridOffset, GridOffset, GridOffset), FColor::Green, true, -1, 0, 10);
			}
		}
	}
}

void ACollisionGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


