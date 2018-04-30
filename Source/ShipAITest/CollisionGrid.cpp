// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionGrid.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"

ACollisionGrid::ACollisionGrid()
{
	PrimaryActorTick.bCanEverTick = false;

	DebugGrid = FVector(0.0f, 0.0f, 0.0f);
	GridOffset = 100.f;
	GridLimitX = 5;
	GridLimitY = 5;
	GridLimitZ = 5;
	bDrawDebug = true;

	Pathing = new Pathfinder();
}

void ACollisionGrid::BeginPlay()
{
	Super::BeginPlay();
	DebugGrid = GetTransform().GetLocation();

	FBox TestBox = FBox(FVector(0.0f, 0.0f, 0.0f), FVector(GridOffset, GridOffset, GridOffset));
	CollisionGrid.Init(TestBox, GridLimitX * GridLimitY * GridLimitZ);

	ExtentValue = (GridOffset / 2) * 0.9f;
	BoxExtents = FVector(ExtentValue, ExtentValue, ExtentValue);
	CollisionBox = FCollisionShape::MakeBox(BoxExtents);

	Pathing->CreateGraph(GridLimitZ, GridLimitX, GridLimitY, GridOffset);

	int32 i, j, k, index = 0;

	for (k = 0; k < GridLimitY; ++k)
	{
		for (j = 0; j < GridLimitZ; ++j)
		{
			for (i = 0; i < GridLimitX; ++i)
			{
				index = i + (j * GridLimitX) + (k * GridLimitX * GridLimitZ);
				CollisionGrid[index] = CollisionGrid[index].MoveTo(DebugGrid + FVector(i * GridOffset, k * GridOffset, j * GridOffset));
				CollisionGrid[index].IsValid = GetWorld()->OverlapAnyTestByChannel(CollisionGrid[index].GetCenter(),
					FQuat(ForceInit), ECC_WorldStatic, CollisionBox);
				if (bDrawDebug)
				{
					if (CollisionGrid[index].IsValid)
					{
						DrawDebugBox(GetWorld(), CollisionGrid[index].GetCenter(),
							BoxExtents, FColor::Red, true, -1, 0, 5);
						Pathing->UpdateGraphFromBrush(Pathfinder::obstacle, index);
					}
					else
					{
						DrawDebugBox(GetWorld(), CollisionGrid[index].GetCenter(),
							BoxExtents, FColor::Yellow, true, -1, 0, 5);
					}
				}
			}
		}
	}

	Pathing->CreatePathDFS();

	//draw green boxes where the path is
	int32 temp, row, column, dep;
	TArray<ATargetPoint*> WaypointList;
	WaypointList.Init(nullptr, Pathing->Path.Num());

	for (i = 0; i < Pathing->Path.Num(); i++)
	{
		temp = Pathing->Path[i];
		dep = temp / (GridLimitX * GridLimitZ);
		temp = temp % (GridLimitX * GridLimitZ);
		row = temp / GridLimitZ;
		column = temp % GridLimitX;
		FVector WorldSpaceLocation = DebugGrid + FVector(GridOffset* column, GridOffset * dep, GridOffset* row);
		WaypointList[i] = GetWorld()->SpawnActor<ATargetPoint>(WorldSpaceLocation, FRotator(0.f, 0.f, 0.f));

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Waypoint: %d"), i+1));

		if (bDrawDebug)
		{
			DrawDebugBox(GetWorld(), WorldSpaceLocation, BoxExtents, FColor::Blue, true, -1, 0, 5);
		}
	}

	MPPath = GetWorld()->SpawnActor<AMovementPath3D>(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	MPPath->SetTargetPoints(WaypointList);

	AIShip = GetWorld()->SpawnActor<AActorVehicle3D>(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	AIShip->SetMovementPath(MPPath);
	AIShip->GetSteeringBehavior()->FollowPathOn();
}

void ACollisionGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


