// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponentSteeringBehavior3D.h"
#include "ActorVehicle3D.h"

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

	//Set Default Weights
	WeightSeek = 1.0f;
	WeightArrive = 1.0f;

	//Larger number slows approach, smaller speeds up to target for Arrive SB
	DecelerationTweaker = 3.f;
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
		FVector TargetSeek = Seek(OwnerVehicle->GetSBTargetLocation());

		//if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
		//	TargetSeek * WeightSeek, FColor(0, 150, 0), true);

		SteeringForce += (TargetSeek * WeightSeek);
	}

	if (IsArriveOn())
	{
		FVector TargetArrive = Arrive(OwnerVehicle->GetSBTargetLocation());

	/*	if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightArrive, FColor(0, 50, 0), true);*/

		SteeringForce += (TargetArrive * WeightArrive);

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

/* ......................................................................

BEGIN BEHAVIOR DEFINITIONS

.......................................................................*/

//------------------------------ Seek ---------------------------------
//
//  this behavior creates a force that steers the agent towards target 
//  
//------------------------------------------------------------------------

FVector UActorComponentSteeringBehavior3D::Seek(FVector TargetPos)
{


	FVector DesiredVelocity = (TargetPos - OwnerVehicle->GetActorLocation());


	DesiredVelocity.Normalize();

	DesiredVelocity *= OwnerVehicle->GetMaxSpeed();

	return (DesiredVelocity - OwnerVehicle->GetVelocity());
}

//------------------------------ Arrive ---------------------------------
//
//  this behavior creates slows down as it approaches target, if far same as flee
//------------------------------------------------------------------------

FVector UActorComponentSteeringBehavior3D::Arrive(FVector TargetPos)
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
