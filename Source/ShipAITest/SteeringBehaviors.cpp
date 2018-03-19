// Fill out your copyright notice in the Description page of Project Settings.

#include "SteeringBehaviors.h"
#include "MovementVehicle.h"
#include "DrawDebugHelpers.h"
#include "MovementPlayer.h"
#include "DrawDebugHelpers.h"
#include "EngineGlobals.h"
#include "MovementGameModeBase.h"
#include "Math/NumericLimits.h"
#include "MovementObstacle.h"
#include "DrawDebugHelpers.h"

//Set default variables
USteeringBehaviors::USteeringBehaviors()
{
	WeightSeek = 1.0f;
	WeightFlee = 1.0f;
	WeightArrive = 1.0f;
	WeightPursuit = 1.0f;
	WeightEvade = 1.0f;
	WanderJitter = 1.0f;
	WeightObstacleAvoidance = 1.0f;
	WeightWallAvoidance = 1.0f;
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
	if (IsWallAvoidanceOn())
	{
	//	SteeringForce += WallAvoidance(m_pVehicle->World()->Walls()) *
		//	WeightWallAvoidance;
	}

	if (IsObstacleAvoidanceOn())
	{
		SteeringForce += ObstacleAvoidance(VehiclePtr->GetGameModeObstacles()) * WeightObstacleAvoidance;
		
		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			SteeringForce, FColor(0, 0, 150));
	}


	if (IsSeekOn())
	{
		FVector2DPlus TargetSeek = Seek(VehiclePtr->GetTarget());
		
		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin( 
			TargetSeek * WeightSeek, FColor(0, 150, 0));

		SteeringForce += (TargetSeek * WeightSeek);
	}

	if (IsFleeOn())
	{
		FVector2DPlus TargetFlee = Flee(VehiclePtr->GetTarget());

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			TargetFlee * WeightFlee, FColor(150, 0, 0));

		SteeringForce += (TargetFlee * WeightFlee);
	}

	if (IsArriveOn())
	{
		FVector2DPlus TargetArrive = Arrive(VehiclePtr->GetTarget());

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightArrive, FColor(0, 50, 0));

		SteeringForce += (TargetArrive * WeightArrive);

	}

	if (IsPursuitOn() && VehiclePtr->ReturnDynamicTargetPtr())
	{
		FVector2DPlus TargetArrive = Pursuit(VehiclePtr->ReturnDynamicTargetPtr());

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightPursuit, FColor(0, 50, 50));

		SteeringForce += (TargetArrive * WeightPursuit);
	}

	if (IsEvadeOn() && VehiclePtr->ReturnDynamicTargetPtr())
	{
		FVector2DPlus TargetArrive = Evade(VehiclePtr->ReturnDynamicTargetPtr());

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightEvade, FColor(50, 0, 0));

		SteeringForce += (TargetArrive * WeightEvade);
	}

	if (IsWanderOn() )
	{
		//Debug lines set in vehicle
		FVector2DPlus TargetArrive = Wander();
		SteeringForce += TargetArrive * WeightWander;
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

void USteeringBehaviors::SetBehaviorWeights(BehaviorTypes BT, float Amount)
{
	switch(BT)
	{
	case BehaviorTypes::Seek:
		WeightSeek = Amount;
		break;

	case BehaviorTypes::Flee:
		WeightFlee = Amount;
		break;

	case BehaviorTypes::Arrive:
		WeightArrive = Amount;
		break;

	case BehaviorTypes::Pursuit:
		WeightPursuit = Amount;
		break;

	case BehaviorTypes::Evade:
		WeightEvade = Amount;
		break;

	case BehaviorTypes::Wander:
		WeightWander = Amount;
		break;

	case BehaviorTypes::ObstacleAvoidance:
		WeightObstacleAvoidance = Amount;
		break;

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

	//move the target into a position WanderDist in front of the agent
	FVector NewTarget =( (VehiclePtr->GetActorForwardVector() * WanderDistance) + VehiclePtr->GetActorLocation()) + FVector(WanderTarget.X, 0.0f, WanderTarget.Y);
	
	
	/** Sets variable to send to Vehicle for debug lines **/
	Target = FVector2DPlus(NewTarget.X, NewTarget.Z);

	return FVector2DPlus(NewTarget.X, NewTarget.Z);
}

//--------------------------- ObstacleAvoidance -------------------------------------
//
//  This behavior keeps the agent away from obstacles
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::ObstacleAvoidance(const TArray<class AMovementObstacle *>& Obstacles)
{
	//the detection box length is proportional to the agent's velocity
	DBoxLength =( VehiclePtr->GetRadius() *2) +
		(VehiclePtr->GetSpeed() / VehiclePtr->GetMaxSpeed()) *
		(VehiclePtr->GetRadius() * 2);

	//tag all obstacles within range of the box for processing
	//m_pVehicle->World()->TagObstaclesWithinViewRange(m_pVehicle, m_dDBoxLength);
	VehiclePtr->GetGameMode()->TagNeighbors(VehiclePtr, DBoxLength);

	//this will keep track of the closest intersecting obstacle (CIB)
	AMovementObstacle* ClosestIntersectingObstacle = NULL;

	//this will be used to track the distance to the CIB
	float DistToClosestIP = TNumericLimits<float>::Max();
	
	//this will record the transformed local coordinates of the CIB
	FVector2DPlus LocalPosOfClosestObstacle;

	for (auto& Ob : Obstacles)
	{
		if (Ob->IsTagged())
		{
			//calculate this obstacle's position in local space
			FVector LocalPosition = VehiclePtr->GetTransform().InverseTransformPosition(Ob->GetActorLocation());

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (LocalPosition.X >= 0)
			{

				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				float ExpandedRadius = Ob->GetRadius() + VehiclePtr->GetRadius();

				if (FMath::Abs(LocalPosition.Z) < ExpandedRadius)
				{
					//now to do a line/circle intersection test. The center of the 
					//circle is represented by (cX, cY). The intersection points are 
					//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
					//We only need to look at the smallest positive value of x because
					//that will be the closest point of intersection.
					float cX = LocalPosition.X;
					float cY = LocalPosition.Z;

					//we only need to calculate the sqrt part of the above equation once
					float SqrtPart = FMath::Sqrt(ExpandedRadius*ExpandedRadius - cY * cY);

					float ip = cX - SqrtPart;

					if (ip <= 0.0)
					{
						ip = cX + SqrtPart;
					}

					//test to see if this is the closest so far. If it is keep a
					//record of the obstacle and its local coordinates
					if (ip < DistToClosestIP)
					{
						DistToClosestIP = ip;

						ClosestIntersectingObstacle = Ob;
					}
				}
			}
		}
	}
	//if we have found an intersecting obstacle, calculate a steering 
	//force away from it
	FVector2DPlus SteeringForce;

	if (ClosestIntersectingObstacle)
	{
		//the closer the agent is to an object, the stronger the 
		//steering force should be
		float Multiplier = 1.0 + (DBoxLength - LocalPosOfClosestObstacle.X) /
			DBoxLength;

		//calculate the lateral force
		SteeringForce.Y = (ClosestIntersectingObstacle->GetRadius() -
			LocalPosOfClosestObstacle.Y)  * Multiplier;

		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const float BrakingWeight = 0.2;

		SteeringForce.X = (ClosestIntersectingObstacle->GetRadius() -
			LocalPosOfClosestObstacle.X) *
			BrakingWeight;

	}

	//finally, convert the steering vector from local to world space
	//Check to see if steering force is 0 (ie. no effect from obstacle)
	if (SteeringForce == FVector2DPlus(0.0f, 0.0f) )
	{
		return FVector2DPlus(0.0f, 0.0f);
	}
	else
	{
	 FVector2DPlus temp = FVector2DPlus(VehiclePtr->GetActorForwardVector().X * SteeringForce.X + VehiclePtr->GetActorLocation2D().X, 
		VehiclePtr->GetActorForwardVector().Z * SteeringForce.Y + VehiclePtr->GetActorLocation2D().Y);

	 if (GEngine)
	 {
		 GEngine->AddOnScreenDebugMessage(1, .1f, FColor::Yellow, FString::Printf(TEXT
		 ("Yranslation is %f, %f"),
			 VehiclePtr->GetActorTransform().GetTranslation().X, VehiclePtr->GetActorTransform().GetTranslation().Y));
	 }

	 return temp;
	}
	
}
	