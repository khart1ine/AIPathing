// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementPath.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMovementPath::AMovementPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bPathLooped = false;
	CurrWaypointIt.Reset();
	bDrawDebugLine = false;

}

// Called when the game starts or when spawned
void AMovementPath::BeginPlay()
{
	Super::BeginPlay();
	if (bDrawDebugLine)
	{
		for (int32 Index = 0; Index != TargetPoints.Num(); ++Index)
		{
			if (Index + 1 < TargetPoints.Num())
			{
				DrawDebugLine(
					GetWorld(),
					TargetPoints[Index]->GetActorLocation(),
					TargetPoints[Index+1]->GetActorLocation(),
					FColor::Black,
					true,  // persistent ( does not go away)
					-1,
					0,
					4	//thickness
				);
			}
			else if (bPathLooped)
			{
				DrawDebugLine(
					GetWorld(),
					TargetPoints[Index]->GetActorLocation(),
					TargetPoints[0]->GetActorLocation(),
					FColor::Black,
					true,  // persistent ( does not go away)
					-1,
					0,
					4	//thickness
				);

			}
		}
	}
}

// Called every frame
void AMovementPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

