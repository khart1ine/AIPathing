// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawGrid3D.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values
ADrawGrid3D::ADrawGrid3D()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DisplayGrid = FVector(0.0f, 0.0f, 0.0f);
	GridOffset = 100;
	GridLimitX = 5;
	GridLimitZ = 5;
	GridLimitY = 5;
	bDrawDebug = true;

	Pathf = new Pathfinder();
}

// Called when the game starts or when spawned
void ADrawGrid3D::BeginPlay()
{
	Super::BeginPlay();
	DisplayGrid = GetTransform().GetLocation();

	ExtentValue = (GridOffset / 2);
	BoxExtents = FVector(ExtentValue * 0.6, ExtentValue * 0.6, ExtentValue * 0.6);

	Pathf->CreateGraph(GridLimitZ, GridLimitX, GridLimitY, GridOffset);
	Pathf->CreatePathDFS();

	if (bDrawDebug)
	{
		int i, j, k;

		for (k = 0; k < GridLimitY; k++)
		{
			for (j = 0; j < GridLimitZ; j++)
			{
				for (i = 0; i < GridLimitX; i++)
				{
					// Draw bounding white boxes
					//DrawDebugBox(GetWorld(), DisplayGrid + FVector(i * GridOffset, k * GridOffset, j * GridOffset),
					//	FVector(ExtentValue, ExtentValue, ExtentValue), FColor::White, true, -1, 0, 5);
					// Draw default non-path cells red
					DrawDebugBox(GetWorld(), DisplayGrid + FVector(GridOffset* i, GridOffset * k, GridOffset* j),
						BoxExtents, FColor::Red, true, -1, 0, 5);
				}
			}
		}

		//draw green boxes where the path is
		int temp, row, column, dep;

		for (i = 0; i < Pathf->Path.Num(); i++)
		{
			temp = Pathf->Path[i];
			dep = temp / (GridLimitX * GridLimitZ);
			temp = temp % (GridLimitX * GridLimitZ);
			row = temp / GridLimitZ;
			column = temp % GridLimitX;

			DrawDebugBox(GetWorld(), DisplayGrid + FVector(GridOffset* column, GridOffset * dep, GridOffset* row),
				BoxExtents, FColor::Green, true, -1, 0, 5);
		}
	}
	
}

// Called every frame
void ADrawGrid3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

