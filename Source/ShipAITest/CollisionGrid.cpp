// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionGrid.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"

ACollisionGrid::ACollisionGrid()
{

	DebugGrid = FVector(0.0f, 0.0f, 0.0f);
	GridOffset = 100.f;
	GridLimit = 5;
	bDrawBoxes = true;

	FBox TestBox = FBox(FVector(0.0f, 0.0f, 0.0f), FVector(GridOffset, GridOffset, GridOffset));
	CollisionGrid.Init(TestBox, pow(GridLimit, 3.f));

	ExtentValue = (GridOffset / 2) * 0.9f;
	BoxExtents = FVector(ExtentValue, ExtentValue, ExtentValue);
	CollisionBox = FCollisionShape::MakeBox(BoxExtents);

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.TickInterval = 100000000.f;
}

void ACollisionGrid::BeginPlay()
{
	Super::BeginPlay();
	DebugGrid = GetTransform().GetLocation();

	int i, j, k, index = 0;

	for (i = 0; i < GridLimit; ++i)
	{
		for (j = 0; j < GridLimit; ++j)
		{
			for (k = 0; k < GridLimit; ++k)
			{
				index = k + j * GridLimit + i * pow(GridLimit, 2);
				CollisionGrid[index] = CollisionGrid[index].MoveTo(DebugGrid + FVector(i * GridOffset, j * GridOffset, k * GridOffset));
				CollisionGrid[index].IsValid = GetWorld()->OverlapAnyTestByChannel(CollisionGrid[index].GetCenter(),
					FQuat(ForceInit), ECC_WorldStatic, CollisionBox);
				if (CollisionGrid[index].IsValid && bDrawBoxes)
				{
					DrawDebugBox(GetWorld(), CollisionGrid[index].GetCenter(),
						BoxExtents, FColor::Red, true, -1, 0, 5);
				}
				else
				{
					DrawDebugBox(GetWorld(), CollisionGrid[index].GetCenter(),
						BoxExtents, FColor::Green, true, -1, 0, 5);
				}
			}
		}
	}
}

void ACollisionGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int i, j, k, index = 0;

	for (i = 0; i < GridLimit; ++i)
	{
		for (j = 0; j < GridLimit; ++j)
		{
			for (k = 0; k < GridLimit; ++k)
			{
				index = k + j * GridLimit + i * pow(GridLimit, 2);
				CollisionGrid[index].IsValid = GetWorld()->OverlapAnyTestByChannel(CollisionGrid[index].GetCenter(),
					FQuat(ForceInit), ECC_WorldStatic, CollisionBox);
				if (CollisionGrid[index].IsValid && bDrawBoxes)
				{
					DrawDebugBox(GetWorld(), CollisionGrid[index].GetCenter(),
						BoxExtents, FColor::Red, true, -1, 0, 5);
				}
				else
				{
					DrawDebugBox(GetWorld(), CollisionGrid[index].GetCenter(),
						BoxExtents, FColor::Green, true, -1, 0, 5);
				}
			}
		}
	}
}


