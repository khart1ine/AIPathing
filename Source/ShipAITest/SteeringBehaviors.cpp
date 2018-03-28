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
#include "MovementPath.h"
#include "MovementWalls.h"

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
	WeightSeparation = 1.0f;
	WeightInterpose = 1.0f;
	WeightHide = 1.0f;
	WeightFollowPath = 1.0f;
	WeightOffsetPursuit = 1.0f;
	LookAheadPursuit = 1.0f;
	WanderTarget = FVector2DPlus(0.0f, 0.0f);
	WanderRadius = 50.f;
	WanderDistance = 200.f;
	DecelerationTweaker = 3.f;
	LookAheadPursuit = 1.f;

	//Delete after debugging
	SBDebugTarget = FVector2DPlus (0.0f,0.0f);

	WallDetectionFeelerLength = 300.0f;

	Feelers.Init(FVector2DPlus(0.0f, 0.0f), 3);
}	

//Returns pure average of all behaviors
FVector2DPlus USteeringBehaviors::CalculateWeightedSum()
{
	if (IsWallAvoidanceOn())
	{
		SteeringForce += WallAvoidance(VehiclePtr->GetGameModeWalls()) *
			WeightWallAvoidance;

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			SteeringForce, FColor(255, 0, 0), true);
	}

	if (IsObstacleAvoidanceOn())
	{
		SteeringForce += ObstacleAvoidance(VehiclePtr->GetGameModeObstacles()) * WeightObstacleAvoidance;
		
		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			ObstacleAvoidance(VehiclePtr->GetGameModeObstacles()) * WeightObstacleAvoidance, FColor(0, 0, 150), true);
	}

	/*if (On(separation))
	{
		SteeringForce += SeparationPlus(m_pVehicle->World()->Agents()) * m_dWeightSeparation;
	}*/

	if (IsSeekOn())
	{
		FVector2DPlus TargetSeek = Seek(VehiclePtr->GetTarget());
		
		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin( 
			TargetSeek * WeightSeek, FColor(0, 150, 0), true);

		SteeringForce += (TargetSeek * WeightSeek);
	}

	if (IsFleeOn())
	{
		FVector2DPlus TargetFlee = Flee(VehiclePtr->GetTarget());

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			TargetFlee * WeightFlee, FColor(150, 0, 0), true);

		SteeringForce += (TargetFlee * WeightFlee);
	}

	if (IsArriveOn())
	{
		FVector2DPlus TargetArrive = Arrive(VehiclePtr->GetTarget());

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightArrive, FColor(0, 50, 0), true);

		SteeringForce += (TargetArrive * WeightArrive);

	}

	if (IsPursuitOn() && VehiclePtr->ReturnDynamicTargetPtr())
	{
		FVector2DPlus TargetArrive = Pursuit(VehiclePtr->ReturnDynamicTargetPtr());

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightPursuit, FColor(0, 50, 50), true);

		SteeringForce += (TargetArrive * WeightPursuit);
	}


	if (IsOffsetPursuitOn())
	{
		//check(VehiclePtr && "pursuit target not assigned");
		//check(!Offset.isZero() && "No offset assigned");

		SteeringForce += OffsetPursuit(VehiclePtr->ReturnDynamicTargetPtr(),Offset) * WeightOffsetPursuit;
	}

	if (IsEvadeOn() && VehiclePtr->ReturnDynamicTargetPtr())
	{
		FVector2DPlus TargetArrive = Evade(VehiclePtr->ReturnDynamicTargetPtr());

		if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightEvade, FColor(50, 0, 0), true);

		SteeringForce += (TargetArrive * WeightEvade);
	}

	if (IsWanderOn() )
	{
		//Debug lines set in vehicle
		FVector2DPlus TargetArrive = Wander();
		SteeringForce += TargetArrive * WeightWander;
	}

	if (IsInterposeOn())
	{
		check(VehiclePtr->GetInterpose1() && VehiclePtr->GetInterpose2() && "Interpose agents not assigned");

		SteeringForce += Interpose(VehiclePtr->GetInterpose1(), VehiclePtr->GetInterpose2()) * WeightInterpose;
	}

	if (IsHideOn())
	{
		check(VehiclePtr->ReturnDynamicTargetPtr() && "Hide target not assigned");

		SteeringForce += Hide(VehiclePtr->ReturnDynamicTargetPtr(), VehiclePtr->GetGameModeObstacles()) * WeightHide;
	}

	if (IsFollowPathOn())
	{
		SteeringForce += FollowPath() * WeightFollowPath;
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

	case BehaviorTypes::WallAvoidance:
		WeightWallAvoidance = Amount;
		break;

	case BehaviorTypes::Separation:
		WeightSeparation = Amount;
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
	FVector2DPlus LocalPosOfClosestObstacle = FVector2DPlus(0.0f, 0.0f);


	for (auto& Ob : Obstacles)
	{
		if (Ob->IsTagged())
		{

			//calculate this obstacle's position in local space
			FVector LocalPosition = FVector(0.0f, 0.0f, 0.0f);
			LocalPosition = VehiclePtr->GetTransform().InverseTransformPosition(Ob->GetActorLocation());

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
	FVector2DPlus SteeringForce = FVector2DPlus(0.0f, 0.0f);

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
	return FVector2DPlus(VehiclePtr->GetActorForwardVector().X * SteeringForce.X + VehiclePtr->GetActorLocation2D().X, 
		VehiclePtr->GetActorForwardVector().Z * SteeringForce.Y + VehiclePtr->GetActorLocation2D().Y);

	}
	
}

//--------------------------- WallAvoidance -------------------------------------
//
//  Returns a steering force that keeps the agent away from the suface of the wall
// that it encounters
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::WallAvoidance(const TArray<FWallType> &Walls)
{
	//Create the feelers to check for walls
	CreateFeelers();

	//Draw Debug for feelers
	if (VehiclePtr->bDrawDebugLines)
	{
		VehiclePtr->PrintDebugLineFromPlayerOrigin(
			Feelers[0], FColor(0, 50, 0), false);

		VehiclePtr->PrintDebugLineFromPlayerOrigin(
			Feelers[1], FColor(0, 50, 0), false);

		VehiclePtr->PrintDebugLineFromPlayerOrigin(
			Feelers[2], FColor(0, 50, 0), false);
	}

	//Holds distance to current wall collision and another to keep the one closest to agent
	float DistToThisIP = 0.0f;
	float DistToClosestIP = TNumericLimits<float>::Max();

	//this will hold an index into the vector of walls
	int32 ClosestWall = -1;

	FVector2DPlus SteeringForce(0.0f, 0.0f); //used to store return
	FVector2DPlus Point;       //used for storing temporary info
	FVector2DPlus ClosestPoint;  //holds the closest intersection point

	//examine each feeler in turn
	for (int32 FeelIndex = 0; FeelIndex != Feelers.Num(); ++FeelIndex)
	{
		//run through each wall checking for any intersection points
		for (int32 WallIndex = 0; WallIndex != Walls.Num(); ++WallIndex)
		{
			if (LineIntersection2D(VehiclePtr->GetActorLocation2D(),
				Feelers[FeelIndex],
				Walls[WallIndex].LineBegin,
				Walls[WallIndex].LineEnd,
				DistToThisIP,
				Point))
			{
		//is this the closest found so far? If so keep a record
				if (DistToThisIP < DistToClosestIP)
				{
					DistToClosestIP = DistToThisIP;

					ClosestWall = WallIndex;

					ClosestPoint = Point;
				}

			}
		}//next wall
		
		if (ClosestWall >= 0)
		{
			//calculate by what distance the projected position of the agent
			//will overshoot the wall
			FVector2DPlus OverShoot = Feelers[FeelIndex] - ClosestPoint;

			//create a force in the direction of the wall normal, with a 
			//magnitude of the overshoot
			SteeringForce = Walls[ClosestWall].LineNormal * OverShoot.Size();
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(103, .1f, FColor::Green, FString::Printf(TEXT("LineIntersects wall is: %d"), ClosestWall));
			}


		}

	}//next feeler
	return SteeringForce;

}

	
//--------------------------- CreateFeelers -------------------------------------
//
//  Creates the antenna utilized by the vehicle for the wall avoidance behavior 
//------------------------------------------------------------------------
void USteeringBehaviors::CreateFeelers()
{
	FVector2DPlus ForwardVector = FVector2DPlus(VehiclePtr->GetActorForwardVector().X * WallDetectionFeelerLength,
		VehiclePtr->GetActorForwardVector().Z * WallDetectionFeelerLength);
	//Feeler straight in front
	Feelers[0] = ForwardVector + VehiclePtr->GetActorLocation2D();

	//Feeler to left
	FVector2DPlus Temp;
	Temp = ForwardVector.GetRotated(30);
	Feelers[1] = Temp + VehiclePtr->GetActorLocation2D();

	//Feeler to the right of plane
	Temp = ForwardVector.GetRotated(-30);
	Feelers[2] = Temp + VehiclePtr->GetActorLocation2D();

}

FVector2DPlus USteeringBehaviors::Interpose(const AMovementVehicle * VehicleA, const AMovementVehicle * VehicleB)
{
	//first we need to figure out where the two agents are going to be at 
	//time T in the future. This is approximated by determining the time
	//taken to reach the mid way point at the current time at at max speed.
	FVector2DPlus MidPoint = (VehicleA->GetActorLocation2D() + VehicleB->GetActorLocation2D()) / 2.0;

	float TimeToReachMidPoint = Vec2DDistance(VehiclePtr->GetActorLocation2D(), MidPoint) /
		VehiclePtr->GetMaxSpeed();

	//now we have T, we assume that agent A and agent B will continue on a
	//straight trajectory and extrapolate to get their future positions
	FVector2DPlus APos = VehicleA->GetActorLocation2D() + VehicleA->Get2DVelocity() * TimeToReachMidPoint;
	FVector2DPlus BPos = VehicleB->GetActorLocation2D() + VehicleB->Get2DVelocity() * TimeToReachMidPoint;

	//calculate the mid point of these predicted positions
	MidPoint = (APos + BPos) / 2.0;

	//how much ahead of the agents position
	FVector2DPlus MidPointForward = MidPoint + (VehiclePtr->GetActorLocation2D() + VehiclePtr->GetActorForwardVector2D() * 100);

	//then steer to Arrive at it
	return Arrive(MidPoint);
}

//--------------------------- Hide ---------------------------------------
//
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::Hide( AMovementPlayer* Hunter, const TArray<AMovementObstacle*>& Obstacles)
{
	float    DistToClosest = TNumericLimits<float>().Max();
	FVector2DPlus BestHidingSpot;

	//std::vector<BaseGameEntity*>::const_iterator curOb = obstacles.begin();
	//std::vector<BaseGameEntity*>::const_iterator closest;
	//const AMovementVehicle* Closest;

	for (auto& Obstacle : Obstacles)
	{
		//calculate the position of the hiding spot for this obstacle
		FVector2DPlus HidingSpot = GetHidingPosition(Obstacle->GetActorLocation2D(),
			Obstacle->GetRadius(),
			Hunter->GetActorLocation2D());

		//work in distance-squared space to find the closest hiding
		//spot to the agent
		float Dist = FVector2DPlus::DistSquared(HidingSpot, VehiclePtr->GetActorLocation2D());

		if (Dist < DistToClosest)
		{
			DistToClosest = Dist;

			BestHidingSpot = HidingSpot;

			//Closest = Obstacle;
		}

	}//next obstacle

	 //if no suitable obstacles found then Evade the hunter
	if (DistToClosest == TNumericLimits<float>().Max())
	{
		return Evade(Hunter);
	}
	
	//GEngine->AddOnScreenDebugMessage(1, .2f, FColor::Red, FString::Printf(TEXT("BestHidingSpot: x: %f, y: %f"), BestHidingSpot.X, BestHidingSpot.Y));

	//else use Arrive on the hiding spot
	return Arrive(BestHidingSpot);
}

//------------------------- GetHidingPosition ----------------------------
//
//  Given the position of a hunter, and the position and radius of
//  an obstacle, this method calculates a position DistanceFromBoundary 
//  away from its bounding radius and directly opposite the hunter
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::GetHidingPosition(const FVector2DPlus& PosOb, const float RadiusOb, const FVector2DPlus& PosHunter)
{
	//calculate how far away the agent is to be from the chosen obstacle's
	//bounding radius
	const float DistanceFromBoundary = 80.0;
	float       DistAway = RadiusOb + DistanceFromBoundary;
	GEngine->AddOnScreenDebugMessage(2, .2f, FColor::Red, FString::Printf(TEXT("Distway: %f, PosObj: %f, %f PosHunter: %f, %f"), DistAway, PosOb.X, PosOb.Y, PosHunter.X, PosHunter.Y));
	//calculate the heading toward the object from the hunter
	FVector2DPlus ToOb = Vec2DNormalize(PosOb - PosHunter);
	GEngine->AddOnScreenDebugMessage(3, .2f, FColor::Red, FString::Printf(TEXT("ToOb: %F, %F"), ToOb.X, ToOb.Y));
	//scale it to size and add to the obstacles position to get
	//the hiding spot.
	return (ToOb * DistAway) + PosOb;
}

//------------------------------- FollowPath -----------------------------
//
//  Given a series of Vector2Ds, this method produces a force that will
//  move the agent along the waypoints in order. The agent uses the
// 'Seek' behavior to move to the next waypoint - unless it is the last
//  waypoint, in which case it 'Arrives'
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::FollowPath()
{

	//move to next target if close enough to current target (working in
	//distance squared space)
	if (FVector2DPlus::DistSquared(VehiclePtr->GetMovementPath()->CurrentWaypoint(), VehiclePtr->GetActorLocation2D()) <
		WaypointPathDistanceSq)
	{
		if (!VehiclePtr->GetMovementPath()->SetNextWaypoint()) // if at last non looping waypoint
		{
			FollowPathOff();
			if (VehiclePtr->bDrawDebugLines && GEngine)
			{
				GEngine->AddOnScreenDebugMessage(20, 5.f, FColor::Red, TEXT("No More Waypoints"));
			}
				VehiclePtr->VelocityZero();//stops player at target
		}
		else
		{
		if (VehiclePtr->bDrawDebugLines && GEngine)
			GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Yellow, TEXT("OntoNextPath"));
		}
	}

	if (!VehiclePtr->GetMovementPath()->Finished() || VehiclePtr->GetMovementPath()->GetPathLooped())
	{
		if (VehiclePtr->bDrawDebugLines && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Green, FString::Printf(TEXT("Going to path: %s"), *VehiclePtr->GetMovementPath()->GetCurrWaypointTarget()->GetName()));
		}
		return Seek(VehiclePtr->GetMovementPath()->CurrentWaypoint());
	}

	else
	{
		if (VehiclePtr->bDrawDebugLines && GEngine)
		{
				GEngine->AddOnScreenDebugMessage(3, .5f, FColor::Yellow, TEXT("Arriving at destination"));
		}
		return Arrive(VehiclePtr->GetMovementPath()->CurrentWaypoint());
	}
}

//------------------------- Offset Pursuit -------------------------------
//
//  Produces a steering force that keeps a vehicle at a specified offset
//  from a leader vehicle
//------------------------------------------------------------------------
FVector2DPlus USteeringBehaviors::OffsetPursuit(const AMovementPlayer* Agent, const FVector2DPlus NewOffset)
{
	//Get ship's direction of movement and convert to angle in degrees

	float VelAngle = FMath::RadiansToDegrees(Agent->Get2DVelocity().GetNormal().GetAngle());

	//rotate offset and cast in world space
	FVector2DPlus WorldOffsetPos = NewOffset.GetRotated(VelAngle) + Agent->GetActorLocation2D();


	FVector2DPlus ToOffset = WorldOffsetPos - VehiclePtr->GetActorLocation2D();

	//the lookahead time is propotional to the distance between the leader
	//and the pursuer; and is inversely proportional to the sum of both
	//agent's velocities
	float LookAheadTime = ToOffset.Size() /
		(VehiclePtr->GetMaxSpeed() + Agent->GetSpeed());

	if (VehiclePtr->bDrawDebugLines) VehiclePtr->PrintDebugLineFromPlayerOrigin(
		WorldOffsetPos + Agent->Get2DVelocity() * LookAheadTime, FColor(50, 0, 0), false);

	//now Arrive at the predicted future position of the offset
	return Arrive(WorldOffsetPos + Agent->Get2DVelocity() * LookAheadTime);
}