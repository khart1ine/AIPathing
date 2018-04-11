// Fill out your copyright notice in the Description page of Project Settings.

#include "Arrive3DSteerBehavComponent.h"
#include "../ActorVehicle3D.h"

// Sets default values for this component's properties
UArrive3DSteerBehavComponent::UArrive3DSteerBehavComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WeightArrive = 1.0f;

	DecelerationTweaker = 3.0f;
}


// Called when the game starts
void UArrive3DSteerBehavComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



//------------------------------ Arrive ---------------------------------
//
//  this behavior creates slows down as it approaches target, if far same as flee
//------------------------------------------------------------------------

FVector UArrive3DSteerBehavComponent::Arrive(AActorVehicle3D* OwnerVehicle, FVector TargetPos)
{
	FVector ToTarget = TargetPos - OwnerVehicle->GetActorLocation();

	//calculate distance to target
	float Dist = ToTarget.Size();


	if (Dist > 20.0f)
	{

		//calculate speed to reach target given desired acceleration
		float Speed = Dist / DecelerationTweaker;

		//make sure the velocity does not exceed the max
		//Speed = Min(Speed, OwnerVehicle->GetMaxSpeed());
		Speed = FMath::Clamp(Speed, 0.0f, OwnerVehicle->GetMaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		FVector DesiredVelocity = ToTarget * (Speed / Dist);


		return (DesiredVelocity - OwnerVehicle->GetVelocity());


	}
	return FVector::ZeroVector;
}

FVector UArrive3DSteerBehavComponent::ArriveWeighted(AActorVehicle3D* OwnerVehicle)
{
	FVector TargetArrive = Arrive(OwnerVehicle, OwnerVehicle->GetSBTargetLocation());

	/*	if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
	TargetArrive * WeightArrive, FColor(0, 50, 0), true);*/

	//if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
	//	TargetSeek * WeightSeek, FColor(0, 150, 0), true);

	return (TargetArrive * WeightArrive);
}
