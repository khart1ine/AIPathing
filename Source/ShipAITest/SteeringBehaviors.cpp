// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringBehaviors.h"
#include "MovementVehicle.h"
#include "DrawDebugHelpers.h"
#include "MovementPlayer.h"
#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"

//Set default variables
USteeringBehaviors::USteeringBehaviors()
{
	WeightSeek = 1.0f;
	WeightFlee = 1.0f;
	WeightArrive = 1.0f;
	WeightPursuit = 1.0f;
	WeightEvade = 1.0f;
	WanderJitter = 1.0f;
	LookAheadPursuit = 1.0f;
	WanderTarget = FVector2DPlus(0.0f, 0.0f);
	WanderRadius = 50.f;
	WanderDistance = 200.f;
	DecelerationTweaker = 3.f;
	LookAheadPursuit = 1.f;

	//Delete after debugging
	SBDebugTarget = FVector2DPlus (0.0f,0.0f);
}	

//Returns pure average of all behaviors
FVector2DPlus USteeringBehaviors::CalculateWeightedSum()
{
	//SteeringForce.x = 0;
	//SteeringForce.y = 0;

	if (CurrentBehavior == BehaviorTypes::Seek)
	{
		FVector2DPlus TargetSeek = Seek(VehiclePtr->GetTarget());
		SteeringForce += (TargetSeek * WeightSeek);
	}

	if (CurrentBehavior == BehaviorTypes::Flee)
	{
		FVector2DPlus TargetFlee = Flee(VehiclePtr->GetTarget());
		SteeringForce += (TargetFlee * WeightFlee);
	}

	if (CurrentBehavior == BehaviorTypes::Arrive)
	{
		FVector2DPlus TargetArrive = Arrive(VehiclePtr->GetTarget());
		SteeringForce += (TargetArrive * WeightArrive);
	}

	if (CurrentBehavior == BehaviorTypes::Pursuit && VehiclePtr->ReturnDynamicTargetPtr())
	{
		FVector2DPlus TargetArrive = Pursuit(VehiclePtr->ReturnDynamicTargetPtr());
		SteeringForce += (TargetArrive * WeightPursuit);
	}

	if (CurrentBehavior == BehaviorTypes::Evade && VehiclePtr->ReturnDynamicTargetPtr())
	{
		FVector2DPlus TargetArrive = Evade(VehiclePtr->ReturnDynamicTargetPtr());
		SteeringForce += (TargetArrive * WeightEvade);
	}

	if (CurrentBehavior == BehaviorTypes::Wander)
	{
		FVector2DPlus TargetArrive = Wander();
		SteeringForce += TargetArrive;
	}

	SteeringForce.Truncate(VehiclePtr->GetMaxForce());

	return SteeringForce;
}

FVector2DPlus USteeringBehaviors::Calculate()
{
	//Resetsteering force to 0,0
	SteeringForce.Zero();

	SteeringForce = CalculateWeightedSum();
	return SteeringForce;
}

void USteeringBehaviors::SetAgent(AMovementVehicle* NewVehiclePtr)
{
	if (NewVehiclePtr != nullptr)
	{
		VehiclePtr = NewVehiclePtr;
	}
	else
	{
		VehiclePtr = nullptr;
	}
}
 
/* ......................................................................

BEGIN BEHAVIOR DEFINITIONS

.......................................................................*/

//------------------------------ Seek ---------------------------------
//
//  this behavior creates a force that steers the agent towards target 
//  
//------------------------------------------------------------------------

FVector2DPlus USteeringBehaviors::Seek(FVector2DPlus TargetPos)
{
	FVector2DPlus DesiredVelocity = (TargetPos - VehiclePtr->GetActorLocation2D());

	DesiredVelocity.Normalize();
	DesiredVelocity *=  VehiclePtr->GetMaxSpeed();

	return (DesiredVelocity - VehiclePtr->Get2DVelocity());
}

//------------------------------ Flee ---------------------------------
//
//  this behavior creates a force that steers the agent away from target 
//  
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::Flee(FVector2DPlus TargetPos)
{
	FVector2DPlus DesiredVelocity = (VehiclePtr->GetActorLocation2D() - TargetPos );
	DesiredVelocity.Normalize();
	DesiredVelocity *= VehiclePtr->GetMaxSpeed();
	return (DesiredVelocity - VehiclePtr->Get2DVelocity());
}

//------------------------------ Arrive ---------------------------------
//
//  this behavior creates slows down as it approaches target, if far same as flee
//------------------------------------------------------------------------

FVector2DPlus USteeringBehaviors::Arrive(FVector2DPlus TargetPos)
{
	FVector2DPlus ToTarget = TargetPos - VehiclePtr->GetActorLocation2D();

	//calculate distance to target
	float Dist = ToTarget.Size();


	if (Dist > 20.0f)
	{

		//calculate speed to reach target given desired acceleration
		float Speed = Dist / DecelerationTweaker;

		//make sure the velocity does not exceed the max
		Speed = Min(Speed, VehiclePtr->GetMaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		FVector2DPlus DesiredVelocity = ToTarget * (Speed / Dist);

		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(10, 15.0f, FColor::Green, FString::Printf(TEXT("DesireVelocity.x is: %f - DesiredVelocity.y is: %f - Dist: %f"), VehiclePtr->Velocity().x, VehiclePtr->Velocity().y, Dist));
		}*/
		return (DesiredVelocity - VehiclePtr->Get2DVelocity());


	}

	return FVector2DPlus(0, 0);
	
}

//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  evader
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::Pursuit(AMovementPlayer* DynamicTarget)
{
//	//if the evader is ahead and facing the agent then we can just seek
//	//for the evader's current position.
	FVector2DPlus ToEvader = DynamicTarget->GetActorLocation2D() - VehiclePtr->GetActorLocation2D();

	float RelativeHeading = FVector2D::DotProduct(VehiclePtr->GetHeading(),DynamicTarget->GetHeading());

	if (((FVector2D::DotProduct(ToEvader, VehiclePtr->GetHeading()) > 0) && (RelativeHeading < -0.95))  || 
		DynamicTarget->GetSpeed() < .1f)  //acos(0.95)=18 degs
	{

		return Seek(DynamicTarget->GetActorLocation2D());
	}

	//Not considered ahead so we predict where the evader will be.

	//the lookahead time is propotional to the distance between the evader
	//and the pursuer; and is inversely proportional to the sum of the
	//agent's velocities
	float LookAheadTime = (ToEvader.Size() / (VehiclePtr->GetMaxSpeed() + DynamicTarget->GetSpeed())) * LookAheadPursuit;

	FVector LocationDebugPoint = FVector((DynamicTarget->GetActorLocation2D() + DynamicTarget->Get2DVelocity() * LookAheadTime).X,
										 0.0f,
										(DynamicTarget->GetActorLocation2D() + DynamicTarget->Get2DVelocity() * LookAheadTime).Y);

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(31, .1f, FColor::Yellow, FString::Printf(TEXT("Player heading to future position: %f, %f"),
	//		LocationDebugPoint.X, LocationDebugPoint.Z));
	//}
	//now seek to the predicted future position of the evader
	return Seek(DynamicTarget->GetActorLocation2D() + DynamicTarget->Get2DVelocity() * LookAheadTime);
}

//------------------------------ Evade ---------------------------------
//
//  this behavior creates a force that steers the agent away from the 
//  evader's future position
//------------------------------------------------------------------------

FVector2DPlus USteeringBehaviors::Evade(AMovementPlayer * DynamicTarget)
{
	//Get location of pursuer
	FVector2DPlus ToPursuer = DynamicTarget->GetActorLocation2D() - VehiclePtr->GetActorLocation2D();

	//The look-ahead time is proportional to the distance between the pursuer
	//and the evader.  It is inversly proportional to the sum of the agent's velocities

	float LookAheadTime = (ToPursuer.Size() / (VehiclePtr->GetMaxSpeed() + DynamicTarget->GetSpeed())) * LookAheadPursuit;

	//now flee from to the predicted future position of the pursuer
	return Flee(DynamicTarget->GetActorLocation2D() + DynamicTarget->Get2DVelocity() * LookAheadTime);
}

//--------------------------- Wander -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::Wander()
{
	//this behavior is dependent on the update rate, so this line must
	//be included when using time independent framerate.
	float JitterThisTimeSlice = WanderJitter * VehiclePtr->GetDeltaTime();

	//first, add a small random vector to the target's position
	WanderTarget += FVector2DPlus(FMath::FRandRange(-1.f, 1.f) * JitterThisTimeSlice,
		FMath::FRandRange(-1.f, 1.f) * JitterThisTimeSlice);

	//reproject this new vector back on to a unit circle
	WanderTarget.Normalize();


	
	//increase the length of the vector to the same as the radius
	//of the wander circle
	WanderTarget *= WanderRadius;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("WanderMagnitue is: %f"), WanderTarget.Size()));
	}

	//move the target into a position WanderDist in front of the agent
	FVector NewTarget =( (VehiclePtr->GetActorForwardVector() * WanderDistance) + VehiclePtr->GetActorLocation()) + FVector(WanderTarget.X, 0.0f, WanderTarget.Y);
	
	
	/** Sets variable to send to Vehicle for debug lines **/
	Target = FVector2DPlus(NewTarget.X, NewTarget.Z);

	return FVector2DPlus(NewTarget.X, NewTarget.Z);

}