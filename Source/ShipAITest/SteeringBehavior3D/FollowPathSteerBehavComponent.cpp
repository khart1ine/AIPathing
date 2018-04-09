// Fill out your copyright notice in the Description page of Project Settings.

#include "FollowPathSteerBehavComponent.h"
#include "Engine.h"

// Sets default values for this component's properties
UFollowPathSteerBehavComponent::UFollowPathSteerBehavComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WeightFollowPath = 1.0f;

	WaypointPathDistanceSq = 500.0f;
}


// Called when the game starts
void UFollowPathSteerBehavComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


//------------------------------- FollowPath -----------------------------
//
//  Given a series of Vector2Ds, this method produces a force that will
//  move the agent along the waypoints in order. The agent uses the
// 'Seek' behavior to move to the next waypoint - unless it is the last
//  waypoint, in which case it 'Arrives'
//------------------------------------------------------------------------
FVector UFollowPathSteerBehavComponent::FollowPath(AActorVehicle3D* OwnerVehicle)
{

	//move to next target if close enough to current target (working in
	//distance squared space)
	if (FVector::DistSquared(OwnerVehicle->GetMovementPath()->CurrentWaypoint(), OwnerVehicle->GetActorLocation()) <
		WaypointPathDistanceSq)
	{
		if (!OwnerVehicle->GetMovementPath()->SetNextWaypoint()) // if at last non looping waypoint
		{
			/*		FollowPathOff();
			if (OwnerVehicle->IsDrawDebugLines() && GEngine)
			{
			GEngine->AddOnScreenDebugMessage(20, 5.f, FColor::Red, TEXT("No More Waypoints"));
			}*/
			//stops player at target
		}
		else
		{
			/*		if (OwnerVehicle->IsDrawDebugLines() && GEngine)
			GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Yellow, TEXT("OntoNextPath"));*/
		}
	}

	if (!OwnerVehicle->GetMovementPath()->Finished() || OwnerVehicle->GetMovementPath()->GetPathLooped())
	{
		/*	if (OwnerVehicle->IsDrawDebugLines() && GEngine)
		{
		GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Green, FString::Printf(TEXT("Going to path: %s"), *OwnerVehicle->GetMovementPath()->GetCurrWaypointTarget()->GetName()));
		}*/

		return OwnerVehicle->SeekSteerBehav->Seek(OwnerVehicle->GetMovementPath()->CurrentWaypoint(), OwnerVehicle);
	}

	else
	{
		/*	if (OwnerVehicle->IsDrawDebugLines() && GEngine)
		{
		GEngine->AddOnScreenDebugMessage(3, .5f, FColor::Yellow, TEXT("Arriving at destination"));
		}*/
		return OwnerVehicle->ArriveSteerBehav->Arrive(OwnerVehicle, OwnerVehicle->GetMovementPath()->CurrentWaypoint());
	}

	//return FVector::ZeroVector;
}

FVector UFollowPathSteerBehavComponent::FollowPathWeighted(AActorVehicle3D * OwnerVehicle)
{
	FVector TargetFollowPath = FollowPath(OwnerVehicle);

	/*	if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
	TargetArrive * WeightArrive, FColor(0, 50, 0), true);*/

	//if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
	//	TargetSeek * WeightSeek, FColor(0, 150, 0), true);

	return (FollowPath(OwnerVehicle) * WeightFollowPath);
}
