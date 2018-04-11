// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponentSteeringBehavior3D.h"
#include "../ActorVehicle3D.h"

// Sets default values for this component's properties
UActorComponentSteeringBehavior3D::UActorComponentSteeringBehavior3D()
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	/** Get Reference to Vehicle that has this component **/
	Owner = GetOwner();
	if (Owner)
	{
		OwnerVehicle = Cast<AActorVehicle3D>(Owner);
	}
}


// Called when the game starts
void UActorComponentSteeringBehavior3D::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

FVector UActorComponentSteeringBehavior3D::CalculateWeightedSum()
{
	if (IsSeekOn())
	{
		SteeringForce += OwnerVehicle->SeekSteerBehav->SeekWeighted(OwnerVehicle);
	}

	if (IsArriveOn())
	{
		SteeringForce += OwnerVehicle->ArriveSteerBehav->ArriveWeighted(OwnerVehicle);
	}

	if (IsFollowPathOn())
	{
		SteeringForce += OwnerVehicle->FollowPathSteerBehav->FollowPathWeighted(OwnerVehicle);
	}

	if (IsWanderOn())
	{
		SteeringForce += OwnerVehicle->WanderPathSteerBehav->WanderWeighted(OwnerVehicle);
	}


	SteeringForce.GetClampedToMaxSize(OwnerVehicle->GetMaxForce());

	//if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
	//	SteeringForce, FColor(255, 0, 0), true);

	return SteeringForce;

}


// Called every frame
void UActorComponentSteeringBehavior3D::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UActorComponentSteeringBehavior3D::Calculate()
{
	//Resetsteering force to 0,0
	SteeringForce = FVector::ZeroVector;

	SteeringForce = CalculateWeightedSum();

	return SteeringForce;
}



