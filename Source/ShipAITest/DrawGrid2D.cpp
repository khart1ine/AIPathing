// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawGrid2D.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
ADrawGrid2D::ADrawGrid2D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DisplayGrid = FVector(0.0f, 0.0f, 0.0f);
	GridOffset = 100;
	GridLimitX = 5;
	GridLimitZ = 5;
	bDrawDebug = true;

	Pathfinder = new Pathfinder2D();
}

// Called when the game starts or when spawned
void ADrawGrid2D::BeginPlay()
{
	Super::BeginPlay();

	ExtentValue = (GridOffset / 2);
	BoxExtents = FVector(ExtentValue * 0.6, ExtentValue * 0.2, ExtentValue * 0.6);

	DisplayGrid = GetTransform().GetLocation();

	ExtentValue = (GridOffset / 2);
	BoxExtents = FVector(ExtentValue * 0.6, ExtentValue * 0.2, ExtentValue * 0.6);

	Pathfinder->CreateGraph(GridLimitZ, GridLimitX, GridOffset);
	//Pathfinder->CreatePathDFS();
	//Pathfinder->CreatePathBFS();
	Pathfinder->CreatePathDijkstra();

	if (bDrawDebug)
	{
		int i, j;

		// Draw Vertical lines of grid
		for (i = 0; i <= GridLimitX; ++i)
		{
			DrawDebugLine(GetWorld(), DisplayGrid + FVector(GridOffset* i, 0.0f, 0.0f),
				DisplayGrid + FVector(GridOffset* i, 0.0f, GridOffset * GridLimitZ), FColor::White,
				true, -1.f, 0, 5.f);
		}

		// draw horizontal lines on grid
		for (i = 0; i <= GridLimitZ; i++)
		{
			DrawDebugLine(GetWorld(), DisplayGrid + FVector(0.0f, 0.0f, GridOffset* i),
				DisplayGrid + FVector(GridOffset* GridLimitX, 0.0f, GridOffset * i), FColor::White,
				true, -1.0f, 0, 5.0f);
		}

		//draw red boxes inside grid cells
		for (i = 0; i < GridLimitX; ++i)
		{
			for (j = 0; j < GridLimitZ; ++j)
			{
				DrawDebugBox(GetWorld(), DisplayGrid + FVector(GridOffset* i, -ExtentValue, GridOffset* j) + ExtentValue,
					BoxExtents, FColor::Red, true, -1, 0, 5);
			}
		}

		//draw green boxes where the path is
		int temp, row, column;

		for (i = 0; i < Pathfinder->Path.Num(); i++)
		{
			temp = Pathfinder->Path[i];
			row = temp / GridLimitZ;
			column = temp % GridLimitX;

			DrawDebugBox(GetWorld(), DisplayGrid + FVector(GridOffset* column, -ExtentValue, GridOffset* row) + ExtentValue,
				BoxExtents, FColor::Green, true, -1, 0, 5);
		}
	}
}

// Called every frame
void ADrawGrid2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FColor ADrawGrid2D::RandomColor()
{
	int RandCol = FMath::RandRange(1, 12);

	switch (RandCol)
	{
	case 1:
		return FColor::Blue;
	case 2:
		return FColor::Cyan;
	case 3:
		return FColor::Emerald;
	case 4:
		return FColor::Green;
	case 5:
		return FColor::Magenta;
	case 6:
		return FColor::Orange;
	case 7:
		return FColor::Purple;
	case 8:
		return FColor::Red;
	case 9:
		return FColor::Silver;
	case 10:
		return FColor::Turquoise;
	case 11:
		return FColor::White;
	case 12:
		return FColor::Yellow;
	default:
		return FColor::Transparent;
	}
}

