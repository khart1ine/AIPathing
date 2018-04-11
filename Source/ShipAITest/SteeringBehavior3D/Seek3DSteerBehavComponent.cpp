// Fill out your copyright notice in the Description page of Project Settings.

#include "Seek3DSteerBehavComponent.h"
#include "../ActorVehicle3D.h"


// Sets default values for this component's properties
USeek3DSteerBehavComponent::USeek3DSteerBehavComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WeightSeek = 1.0f;
}


// Called when the game starts
void USeek3DSteerBehavComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//------------------------------ Seek ---------------------------------
//
//  this behavior creates a force that steers the agent towards target 
//  
//------------------------------------------------------------------------
//
FVector USeek3DSteerBehavComponent::Seek(FVector TargetPos, AActorVehicle3D* OwnerVehicle)
{

	FVector DesiredVelocity = (TargetPos - OwnerVehicle->GetActorLocation());


	DesiredVelocity.Normalize();

	DesiredVelocity *= OwnerVehicle->GetMaxSpeed();

	return (DesiredVelocity - OwnerVehicle->GetVelocity());
}

FVector USeek3DSteerBehavComponent::SeekWeighted(AActorVehicle3D* OwnerVehicle)
{
	FVector TargetSeek = Seek(OwnerVehicle->GetSBTargetLocation(), OwnerVehicle);

	//if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
	//	TargetSeek * WeightSeek, FColor(0, 150, 0), true);

	return (TargetSeek * WeightSeek);
}
