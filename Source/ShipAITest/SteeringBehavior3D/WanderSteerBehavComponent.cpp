// Fill out your copyright notice in the Description page of Project Settings.

#include "WanderSteerBehavComponent.h"
#include "../ActorVehicle3D.h"


// Sets default values for this component's properties
UWanderSteerBehavComponent::UWanderSteerBehavComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WeightWander = 1.0f;

	//How much randomness in the Wander movement SB
	WanderJitter = 1000.0f;
	WanderDistance = 250.0f;
	WanderRadius = 100.0f;
}


FVector UWanderSteerBehavComponent::Wander(AActorVehicle3D* OwnerVehicle)
{
	//this behavior is dependent on the update rate, so this line must
	//be included when using time independent framerate.
	float JitterThisTimeSlice = WanderJitter * OwnerVehicle->GetDeltaTime();

	//first, add a small random vector to the target's position
	WanderTarget += FVector(FMath::FRandRange(-1.f, 1.f) * JitterThisTimeSlice,
		FMath::FRandRange(-1.f, 1.f) * JitterThisTimeSlice, FMath::FRandRange(-1.f, 1.f) * JitterThisTimeSlice);

	//reproject this new vector back on to a unit circle
	WanderTarget.Normalize();


	//increase the length of the vector to the same as the radius
	//of the wander circle
	WanderTarget *= WanderRadius;

	//move the target into a position WanderDist in front of the agent
	FVector NewTarget = ((OwnerVehicle->GetActorForwardVector() * WanderDistance) + OwnerVehicle->GetActorLocation()) + FVector(WanderTarget.X, 0.0f, WanderTarget.Y);


	/** Sets variable to send to Vehicle for debug lines **/
	//DebugTarget = FVector2DPlus(NewTarget.X, NewTarget.Z);

	//return FVector2DPlus(NewTarget.X, NewTarget.Z);
	return WanderTarget;
}

FVector UWanderSteerBehavComponent::WanderWeighted(AActorVehicle3D * OwnerVehicle)
{
	FVector WanderSeek = Wander(OwnerVehicle);

	//if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
	//	TargetSeek * WeightSeek, FColor(0, 150, 0), true);

	return (WanderSeek * WeightWander);
}

// Called when the game starts
void UWanderSteerBehavComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


