// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponentSteeringBehavior.h"
#include "ActorVehicle.h"
#include "DrawDebugHelpers.h"
#include "PlayerPawn.h"
#include "MovementGameModeBase.h"
#include "MovementObstacle.h"


// Sets default values for this component's properties
UActorComponentSteeringBehavior::UActorComponentSteeringBehavior()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Owner = GetOwner();
	if (Owner)
	{
		OwnerVehicle = Cast<AActorVehicle>(Owner);
	}

	//Set Default Weights
	WeightSeek = 1.0f;
	WeightFlee = 1.0f;
	WeightArrive = 1.0f;
	WeightPursuit = 1.0f;
	WeightEvade = 1.0f;
	WeightWander = 1.0f;
	WeightObstacleAvoidance = 1.0f;
	WeightWallAvoidance = 1.0f;
	WeightInterpose = 1.0f;
	WeightHide = 1.0f;
	WeightFollowPath = 1.0f;
	WeightOffsetPursuit = 1.0f;
	WeightSeparation = 1.0f;
	WeightCohesion = 1.0f;
	WeightAlignment = 1.0f;


	//Larger number slows approach, smaller speeds up to target for Arrive SB
	DecelerationTweaker = 3.f;
	
	//how far in front of the player's vector should the vehicle target for Pursuit and & Evade SB
	LookAheadPursuit = 1.f;

	//How much randomness in the Wander movement SB
	WanderJitter = 1000.0f;
	WanderDistance = 250.0f;
	WanderRadius = 100.0f;

	//How far out the box extends for Obstacle Avoidance
	DBoxLength = 200.0f;

	//How far ahead the minimum box is for vehicle in obstacle avoidance
	MinDBoxLength = 100.0f;

	WallDetectionFeelerLength = 200.0f;

	//Stores the three feelers for the Wall Avoidance SB
	Feelers.Init(FVector2DPlus(0.0f, 0.0f), 3);

	WaypointPathDistanceSq = 500.0f;
}


// Called when the game starts
void UActorComponentSteeringBehavior::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UActorComponentSteeringBehavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector2DPlus UActorComponentSteeringBehavior::Calculate()
{
	//Resetsteering force to 0,0
	SteeringForce.Zero();

	//Tag neighbors if any of the 2 gruop behaviors are switched on
	if (IsSeparationOn() || IsAlignmentOn() || IsCohesionOn())
	{
		//tag all obstacles within range of the box for processing
		//m_pVehicle->World()->TagObstaclesWithinViewRange(m_pVehicle, m_dDBoxLength);
		OwnerVehicle->GetGameMode()->TagAgentNeighbors(OwnerVehicle, DBoxLength);
	}

	SteeringForce = CalculateWeightedSum();

	return SteeringForce;
}

//Returns pure average of all behaviors
FVector2DPlus UActorComponentSteeringBehavior::CalculateWeightedSum()
{

	if (IsSeekOn())
	{
		FVector2DPlus TargetSeek = Seek(OwnerVehicle->GetSBTargetLocation());

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			TargetSeek * WeightSeek, FColor(0, 150, 0), true);

		SteeringForce += (TargetSeek * WeightSeek);
	}

	if (IsFleeOn())
	{
		FVector2DPlus TargetFlee = Flee(OwnerVehicle->GetSBTargetLocation());

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			TargetFlee * WeightFlee, FColor(150, 0, 0), true);

		SteeringForce += (TargetFlee * WeightFlee);
	}

	if (IsArriveOn())
	{
		FVector2DPlus TargetArrive = Arrive(OwnerVehicle->GetSBTargetLocation());

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightArrive, FColor(0, 50, 0), true);

		SteeringForce += (TargetArrive * WeightArrive);

	}

	if (IsPursuitOn())
	{
		FVector2DPlus TargetArrive = Pursuit(OwnerVehicle->GetTargetPtr(), OwnerVehicle->IsActorNPC());

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightPursuit, FColor(0, 50, 50), true);

		SteeringForce += (TargetArrive * WeightPursuit);
	}

	if (IsEvadeOn())
	{
		FVector2DPlus TargetArrive = Evade(OwnerVehicle->GetTargetPtr(), OwnerVehicle->IsActorNPC());

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			TargetArrive * WeightEvade, FColor(50, 0, 0), true);

		SteeringForce += (TargetArrive * WeightEvade);
	}

	if (IsWanderOn())
	{
		//Debug lines set in vehicle
		FVector2DPlus TargetArrive = Wander();
		SteeringForce += TargetArrive * WeightWander;

		if (OwnerVehicle->IsDrawDebugLines())
		{
			DrawDebugCircle(
				GetWorld(),
				(OwnerVehicle->GetActorForwardVector()*WanderDistance) + OwnerVehicle->GetActorLocation(),
				WanderRadius,
				32,
				FColor(255, 0, 0),
				false,
				-1,
				0,
				3,
				FVector(1, 0, 0),
				FVector(0, 0, 1),
				false
				);

			DrawDebugLine(
				GetWorld(),
				OwnerVehicle->GetActorForwardVector()*WanderDistance + OwnerVehicle->GetActorLocation(),
				FVector(DebugTarget.X, 0.0f, DebugTarget.Y),
				FColor(0,50,0),
				false,
				.1f,
				1,
				3.0f
			);

			OwnerVehicle->PrintDebugLineFromPlayerOrigin(
				TargetArrive * WeightWander + OwnerVehicle->Component2D->GetActorLocation2D(), FColor(50, 0, 0), false);
			}
	}

	if (IsObstacleAvoidanceOn())
	{
		SteeringForce += ObstacleAvoidance(OwnerVehicle->GetGameModeObstacles()) * WeightObstacleAvoidance;

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			ObstacleAvoidance(OwnerVehicle->GetGameModeObstacles()) * WeightObstacleAvoidance, FColor(0, 0, 150), true);
	}

	if (IsWallAvoidanceOn())
	{
		SteeringForce += WallAvoidance(OwnerVehicle->GetGameModeWalls()) *
			WeightWallAvoidance;

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			WallAvoidance(OwnerVehicle->GetGameModeWalls())*
			WeightWallAvoidance, FColor(255, 0, 0), true);
	}

	if (IsInterposeOn())
	{
		check(OwnerVehicle->GetInterpose1() && OwnerVehicle->GetInterpose2() && "Interpose agents not assigned");

		SteeringForce += Interpose(OwnerVehicle->GetInterpose1(), OwnerVehicle->GetInterpose2()) * WeightInterpose;

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			Interpose(OwnerVehicle->GetInterpose1(), OwnerVehicle->GetInterpose2()) * WeightInterpose, FColor(0, 0, 0), true);
	}

	if (IsHideOn())
	{
		check(OwnerVehicle->GetTargetPtr() && "Hide target not assigned");
		APlayerPawn* PP = Cast<APlayerPawn>(OwnerVehicle->GetTargetPtr());
		SteeringForce += Hide(PP, OwnerVehicle->GetGameModeObstacles()) * WeightHide;
	}

	if (IsFollowPathOn())
	{
		SteeringForce += FollowPath() * WeightFollowPath;
	}

	if (IsOffsetPursuitOn())
	{
		//check(VehiclePtr && "pursuit target not assigned");
		//check(!Offset.isZero() && "No offset assigned");

		SteeringForce += OffsetPursuit(OwnerVehicle->GetTargetPtr(), Offset) * WeightOffsetPursuit;
	}

	if (IsSeparationOn())
	{
		SteeringForce += Separation(OwnerVehicle->GetAgentsInLevel()) * WeightSeparation;

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			Separation(OwnerVehicle->GetAgentsInLevel()) * WeightSeparation, FColor(0, 0, 0), true);
	}

	if (IsCohesionOn())
	{
		SteeringForce += Cohesion(OwnerVehicle->GetAgentsInLevel()) * WeightCohesion;

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			Cohesion(OwnerVehicle->GetAgentsInLevel()) * WeightCohesion, FColor(0, 0, 0), true);
	}

	if (IsAlignmentOn())
	{
		SteeringForce += Alignment(OwnerVehicle->GetAgentsInLevel()) * WeightAlignment;

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			Alignment(OwnerVehicle->GetAgentsInLevel()) * WeightAlignment, FColor(0, 0, 0), true);
	}

	SteeringForce.Truncate(OwnerVehicle->GetMaxForce());

	if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
		SteeringForce, FColor(255, 0, 0), true);

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

FVector2DPlus UActorComponentSteeringBehavior::Seek(FVector2DPlus TargetPos)
{
	FVector2DPlus DesiredVelocity = (TargetPos - OwnerVehicle->Component2D->GetActorLocation2D());
	
	DesiredVelocity.Normalize();
	DesiredVelocity *= OwnerVehicle->GetMaxSpeed();

	return (DesiredVelocity - OwnerVehicle->Get2DVelocity());
}

//------------------------------ Flee ---------------------------------
//
//  this behavior creates a force that steers the agent away from target 
//  
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::Flee(FVector2DPlus TargetPos)
{
	FVector2DPlus DesiredVelocity = (OwnerVehicle->Component2D->GetActorLocation2D() - TargetPos);
	DesiredVelocity.Normalize();
	DesiredVelocity *= OwnerVehicle->GetMaxSpeed();
	return (DesiredVelocity - OwnerVehicle->Get2DVelocity());
}

//------------------------------ Arrive ---------------------------------
//
//  this behavior creates slows down as it approaches target, if far same as flee
//------------------------------------------------------------------------

FVector2DPlus UActorComponentSteeringBehavior::Arrive(FVector2DPlus TargetPos)
{
	FVector2DPlus ToTarget = TargetPos - OwnerVehicle->Component2D->GetActorLocation2D();

	//calculate distance to target
	float Dist = ToTarget.Size();


	if (Dist > 20.0f)
	{

		//calculate speed to reach target given desired acceleration
		float Speed = Dist / DecelerationTweaker;

		//make sure the velocity does not exceed the max
		Speed = Min(Speed, OwnerVehicle->GetMaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		FVector2DPlus DesiredVelocity = ToTarget * (Speed / Dist);


		return (DesiredVelocity - OwnerVehicle->Get2DVelocity());


	}
	return FVector2DPlus(0, 0);
}

//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  evader
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::Pursuit(class AActor* TargetPos, bool IsNPC)
{
	APlayerPawn* PlayerPawnPtr = nullptr;
	AActorVehicle* ActorVehiclePtr = nullptr;

	if (IsNPC)
	{
		ActorVehiclePtr = Cast<AActorVehicle>(TargetPos);
	}
	else
	{
		PlayerPawnPtr = Cast<APlayerPawn>(TargetPos);
	}

	if (ActorVehiclePtr)
	{
		//	//if the evader is ahead and facing the agent then we can just seek
		//	//for the evader's current position.
		FVector2DPlus ToEvader = ActorVehiclePtr->Component2D->GetActorLocation2D() - OwnerVehicle->Component2D->GetActorLocation2D();

		float RelativeHeading = FVector2D::DotProduct(OwnerVehicle->GetHeading(), ActorVehiclePtr->GetHeading());

		if (((FVector2D::DotProduct(ToEvader, OwnerVehicle->GetHeading()) > 0) && (RelativeHeading < -0.95)) ||
			ActorVehiclePtr->GetSpeed() < .1f)  //acos(0.95)=18 degs
		{

			return Seek(ActorVehiclePtr->Component2D->GetActorLocation2D());
		}

		//Not considered ahead so we predict where the evader will be.

		//the lookahead time is propotional to the distance between the evader
		//and the pursuer; and is inversely proportional to the sum of the
		//agent's velocities
		float LookAheadTime = (ToEvader.Size() / (OwnerVehicle->GetMaxSpeed() + ActorVehiclePtr->GetSpeed())) * LookAheadPursuit;

		FVector LocationDebugPoint = FVector((ActorVehiclePtr->Component2D->GetActorLocation2D() + ActorVehiclePtr->Get2DVelocity() * LookAheadTime).X,
			0.0f,
			(ActorVehiclePtr->Component2D->GetActorLocation2D() + ActorVehiclePtr->Get2DVelocity() * LookAheadTime).Y);

		//now seek to the predicted future position of the evader
		return Seek(ActorVehiclePtr->Component2D->GetActorLocation2D() + ActorVehiclePtr->Get2DVelocity() * LookAheadTime);
	}

	else if (PlayerPawnPtr)
	{
		//	//if the evader is ahead and facing the agent then we can just seek
		//	//for the evader's current position.
		FVector2DPlus ToEvader = PlayerPawnPtr->Component2D->GetActorLocation2D() - OwnerVehicle->Component2D->GetActorLocation2D();

		float RelativeHeading = FVector2D::DotProduct(OwnerVehicle->GetHeading(), PlayerPawnPtr->Get2DVelocity());

		if (((FVector2D::DotProduct(ToEvader, OwnerVehicle->GetHeading()) > 0) && (RelativeHeading < -0.95)) ||
			PlayerPawnPtr->GetSpeed() < .1f)  //acos(0.95)=18 degs
		{

			return Seek(PlayerPawnPtr->Component2D->GetActorLocation2D());
		}

		//Not considered ahead so we predict where the evader will be.

		//the lookahead time is propotional to the distance between the evader
		//and the pursuer; and is inversely proportional to the sum of the
		//agent's velocities
		float LookAheadTime = (ToEvader.Size() / (OwnerVehicle->GetMaxSpeed() + PlayerPawnPtr->GetSpeed())) * LookAheadPursuit;

		FVector LocationDebugPoint = FVector((PlayerPawnPtr->Component2D->GetActorLocation2D() + PlayerPawnPtr->Get2DVelocity() * LookAheadTime).X,
			0.0f,
			(PlayerPawnPtr->Component2D->GetActorLocation2D() + PlayerPawnPtr->Get2DVelocity() * LookAheadTime).Y);

		//now seek to the predicted future position of the evader
		return Seek(PlayerPawnPtr->Component2D->GetActorLocation2D() + PlayerPawnPtr->Get2DVelocity() * LookAheadTime);
	}
	return FVector2DPlus(0.0f, 0.0f); 
}

//------------------------------ Evade ---------------------------------
//
//  this behavior creates a force that steers the agent away from the 
//  evader's future position
//------------------------------------------------------------------------

FVector2DPlus UActorComponentSteeringBehavior::Evade(class AActor* TargetPos, bool IsNPC)
{
	APlayerPawn* PlayerPawnPtr = nullptr;
	AActorVehicle* ActorVehiclePtr = nullptr;

	if (IsNPC)
	{
		ActorVehiclePtr = Cast<AActorVehicle>(TargetPos);
	}
	else
	{
		PlayerPawnPtr = Cast<APlayerPawn>(TargetPos);
	}

	if (PlayerPawnPtr)
	{
		//Get location of pursuer
		FVector2DPlus ToPursuer = PlayerPawnPtr->Component2D->GetActorLocation2D() - OwnerVehicle->Component2D->GetActorLocation2D();

		//The look-ahead time is proportional to the distance between the pursuer
		//and the evader.  It is inversly proportional to the sum of the agent's velocities

		float LookAheadTime = (ToPursuer.Size() / (OwnerVehicle->GetMaxSpeed() + PlayerPawnPtr->GetSpeed())) * LookAheadPursuit;

		//now flee from to the predicted future position of the pursuer
		return Flee(PlayerPawnPtr->Component2D->GetActorLocation2D() + PlayerPawnPtr->Get2DVelocity() * LookAheadTime);
	}

	else if (ActorVehiclePtr)
	{
		//Get location of pursuer
		FVector2DPlus ToPursuer = ActorVehiclePtr->Component2D->GetActorLocation2D() - OwnerVehicle->Component2D->GetActorLocation2D();

		//The look-ahead time is proportional to the distance between the pursuer
		//and the evader.  It is inversly proportional to the sum of the agent's velocities

		float LookAheadTime = (ToPursuer.Size() / (OwnerVehicle->GetMaxSpeed() + ActorVehiclePtr->GetSpeed())) * LookAheadPursuit;

		//now flee from to the predicted future position of the pursuer
		return Flee(ActorVehiclePtr->Component2D->GetActorLocation2D() + ActorVehiclePtr->Get2DVelocity() * LookAheadTime);
	}
	return FVector2DPlus(0.0f, 0.0f);
}

//--------------------------- Wander -------------------------------------
//
//  This behavior makes the agent wander about randomly
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::Wander()
{
	//this behavior is dependent on the update rate, so this line must
	//be included when using time independent framerate.
	float JitterThisTimeSlice = WanderJitter * OwnerVehicle->GetDeltaTime();

	//first, add a small random vector to the target's position
	WanderTarget += FVector2DPlus(FMath::FRandRange(-1.f, 1.f) * JitterThisTimeSlice,
		FMath::FRandRange(-1.f, 1.f) * JitterThisTimeSlice);

	//reproject this new vector back on to a unit circle
	WanderTarget.Normalize();



	//increase the length of the vector to the same as the radius
	//of the wander circle
	WanderTarget *= WanderRadius;

	//move the target into a position WanderDist in front of the agent
	FVector NewTarget = ((OwnerVehicle->GetActorForwardVector() * WanderDistance) + OwnerVehicle->GetActorLocation()) + FVector(WanderTarget.X, 0.0f, WanderTarget.Y);


	/** Sets variable to send to Vehicle for debug lines **/
	DebugTarget = FVector2DPlus(NewTarget.X, NewTarget.Z);

	//return FVector2DPlus(NewTarget.X, NewTarget.Z);
	return WanderTarget;
}

//--------------------------- ObstacleAvoidance -------------------------------------
//
//  This behavior keeps the agent away from obstacles
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::ObstacleAvoidance(const TArray<class AMovementObstacle *>& Obstacles)
{
	//the detection box length is proportional to the agent's velocity
	DBoxLength = MinDBoxLength +
		(OwnerVehicle->GetSpeed() / OwnerVehicle->GetMaxSpeed()) *
		MinDBoxLength;

	//tag all obstacles within range of the box for processing
	//m_pVehicle->World()->TagObstaclesWithinViewRange(m_pVehicle, m_dDBoxLength);
	OwnerVehicle->GetGameMode()->TagNeighbors(OwnerVehicle, DBoxLength);

	//this will keep track of the closest intersecting obstacle (CIB)
	AMovementObstacle* ClosestIntersectingObstacle = nullptr;

	//this will be used to track the distance to the CIB
	float DistToClosestIP = TNumericLimits<float>::Max();

	//this will record the transformed local coordinates of the CIB
	FVector2DPlus LocalPosOfClosestObstacle = FVector2DPlus(0.0f, 0.0f);


	for (auto& Ob : Obstacles)
	{
		if (Ob->GetComponent2D()->IsTagged())
		{

			//calculate this obstacle's position in local space
			FVector LocalPosition = FVector(0.0f, 0.0f, 0.0f);
			LocalPosition = OwnerVehicle->GetTransform().InverseTransformPosition(Ob->GetActorLocation());

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (LocalPosition.X >= 0)
			{

				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				float ExpandedRadius = Ob->GetComponent2D()->GetRadius() + OwnerVehicle->Component2D->GetRadius();

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

						LocalPosOfClosestObstacle = Ob->GetComponent2D()->GetActorLocation2D();
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
		SteeringForce.Y = (ClosestIntersectingObstacle->GetComponent2D()->GetRadius() -
			LocalPosOfClosestObstacle.Y)  * Multiplier;

		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const float BrakingWeight = 0.2;

		SteeringForce.X = (ClosestIntersectingObstacle->GetComponent2D()->GetRadius() -
			LocalPosOfClosestObstacle.X) *
			BrakingWeight;

	}

	//finally, convert the steering vector from local to world space
	//Check to see if steering force is 0 (ie. no effect from obstacle)
	if (SteeringForce == FVector2DPlus(0.0f, 0.0f))
	{
		return FVector2DPlus(0.0f, 0.0f);
	}
	else
	{

		//return FVector2DPlus(OwnerVehicle->GetActorForwardVector().X * SteeringForce.X + OwnerVehicle->Component2D->GetActorLocation2D().X,
			//OwnerVehicle->GetActorForwardVector().Z * SteeringForce.Y + OwnerVehicle->Component2D->GetActorLocation2D().Y);
		return SteeringForce.GetRotated(OwnerVehicle->GetAngleInDegrees());
	}

}


//--------------------------- WallAvoidance -------------------------------------
//
//  Returns a steering force that keeps the agent away from the suface of the wall
// that it encounters
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::WallAvoidance(const TArray<FWallType> &Walls)
{
	//Create the feelers to check for walls
	CreateFeelers();

	//Draw Debug for feelers
	if (OwnerVehicle->IsDrawDebugLines())
	{
		OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			Feelers[0], FColor(0, 50, 0), false);

		OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			Feelers[1], FColor(0, 50, 0), false);

		OwnerVehicle->PrintDebugLineFromPlayerOrigin(
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
			if (LineIntersection2D(OwnerVehicle->Component2D->GetActorLocation2D(),
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
void UActorComponentSteeringBehavior::CreateFeelers()
{
	FVector2DPlus ForwardVector = FVector2DPlus(OwnerVehicle->GetActorForwardVector().X * WallDetectionFeelerLength,
		OwnerVehicle->GetActorForwardVector().Z * WallDetectionFeelerLength);
	//Feeler straight in front
	Feelers[0] = ForwardVector + OwnerVehicle->Component2D->GetActorLocation2D();

	//Feeler to left
	FVector2DPlus Temp;
	Temp = ForwardVector.GetRotated(30);
	Feelers[1] = Temp + OwnerVehicle->Component2D->GetActorLocation2D();

	//Feeler to the right of plane
	Temp = ForwardVector.GetRotated(-30);
	Feelers[2] = Temp + OwnerVehicle->Component2D->GetActorLocation2D();

}

//--------------------------- Interpose ----------------------------------
//
//  Given two agents, this method returns a force that attempts to 
//  position the vehicle between them
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::Interpose(const AActorVehicle* VehicleA, const AActorVehicle* VehicleB)
{
	//first we need to figure out where the two agents are going to be at 
	//time T in the future. This is approximated by determining the time
	//taken to reach the mid way point at the current time at at max speed.
	FVector2DPlus MidPoint = (VehicleA->Component2D->GetActorLocation2D() + VehicleB->Component2D->GetActorLocation2D()) / 2.0;

	float TimeToReachMidPoint = Vec2DDistance(OwnerVehicle->Component2D->GetActorLocation2D(), MidPoint) /
		OwnerVehicle->GetMaxSpeed();

	//now we have T, we assume that agent A and agent B will continue on a
	//straight trajectory and extrapolate to get their future positions
	FVector2DPlus APos = VehicleA->Component2D->GetActorLocation2D() + VehicleA->Get2DVelocity() * TimeToReachMidPoint;
	FVector2DPlus BPos = VehicleB->Component2D->GetActorLocation2D() + VehicleB->Get2DVelocity() * TimeToReachMidPoint;

	//calculate the mid point of these predicted positions
	MidPoint = (APos + BPos) / 2.0;

	//how much ahead of the agents position
	FVector2DPlus MidPointForward = MidPoint + (OwnerVehicle->Component2D->GetActorLocation2D() + OwnerVehicle->Component2D->GetActorForwardVector2D() * 100);

	//then steer to Arrive at it
	return Arrive(MidPoint);
}

//--------------------------- Hide ---------------------------------------
//
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::Hide(APlayerPawn* Hunter, const TArray<AMovementObstacle*>& Obstacles)
{
	float    DistToClosest = TNumericLimits<float>().Max();
	FVector2DPlus BestHidingSpot;

	//std::vector<BaseGameEntity*>::const_iterator curOb = obstacles.begin();
	//std::vector<BaseGameEntity*>::const_iterator closest;
	//const AMovementVehicle* Closest;

	for (auto& Obstacle : Obstacles)
	{
		//calculate the position of the hiding spot for this obstacle
		FVector2DPlus HidingSpot = GetHidingPosition(Obstacle->GetComponent2D()->GetActorLocation2D(),
			Obstacle->GetComponent2D()->GetRadius(),
			Hunter->Component2D->GetActorLocation2D());

		//work in distance-squared space to find the closest hiding
		//spot to the agent
		float Dist = FVector2DPlus::DistSquared(HidingSpot, OwnerVehicle->Component2D->GetActorLocation2D());

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
		return Evade(Hunter, false);
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
FVector2DPlus UActorComponentSteeringBehavior::GetHidingPosition(const FVector2DPlus& PosOb, const float RadiusOb, const FVector2DPlus& PosHunter)
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
FVector2DPlus UActorComponentSteeringBehavior::FollowPath()
{

	//move to next target if close enough to current target (working in
	//distance squared space)
	if (FVector2DPlus::DistSquared(OwnerVehicle->GetMovementPath()->CurrentWaypoint(), OwnerVehicle->Component2D->GetActorLocation2D()) <
		WaypointPathDistanceSq)
	{
		if (!OwnerVehicle->GetMovementPath()->SetNextWaypoint()) // if at last non looping waypoint
		{
			FollowPathOff();
			if (OwnerVehicle->IsDrawDebugLines() && GEngine)
			{
				GEngine->AddOnScreenDebugMessage(20, 5.f, FColor::Red, TEXT("No More Waypoints"));
			}
			OwnerVehicle->VelocityZero();//stops player at target
		}
		else
		{
			if (OwnerVehicle->IsDrawDebugLines() && GEngine)
				GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Yellow, TEXT("OntoNextPath"));
		}
	}

	if (!OwnerVehicle->GetMovementPath()->Finished() || OwnerVehicle->GetMovementPath()->GetPathLooped())
	{
		if (OwnerVehicle->IsDrawDebugLines() && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, .5f, FColor::Green, FString::Printf(TEXT("Going to path: %s"), *OwnerVehicle->GetMovementPath()->GetCurrWaypointTarget()->GetName()));
		}
		return Seek(OwnerVehicle->GetMovementPath()->CurrentWaypoint());
	}

	else
	{
		if (OwnerVehicle->IsDrawDebugLines() && GEngine)
		{
			GEngine->AddOnScreenDebugMessage(3, .5f, FColor::Yellow, TEXT("Arriving at destination"));
		}
		return Arrive(OwnerVehicle->GetMovementPath()->CurrentWaypoint());
	}
}

//------------------------- Offset Pursuit -------------------------------
//
//  Produces a steering force that keeps a vehicle at a specified offset
//  from a leader vehicle
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::OffsetPursuit( AActor* Agent, const FVector2DPlus NewOffset)
{
	APlayerPawn* PlayerPawnPtr = nullptr;
	AActorVehicle* ActorVehiclePtr = nullptr;

	ActorVehiclePtr = Cast<AActorVehicle>(Agent);
	PlayerPawnPtr = Cast<APlayerPawn>(Agent);

	if (ActorVehiclePtr)
	{
		//Get ship's direction of movement and convert to angle in degrees

		float VelAngle = FMath::RadiansToDegrees(ActorVehiclePtr->Get2DVelocity().GetNormal().GetAngle());

		//rotate offset and cast in world space
		FVector2DPlus WorldOffsetPos = NewOffset.GetRotated(VelAngle) + ActorVehiclePtr->Component2D->GetActorLocation2D();


		FVector2DPlus ToOffset = WorldOffsetPos - OwnerVehicle->Component2D->GetActorLocation2D();

		//the lookahead time is propotional to the distance between the leader
		//and the pursuer; and is inversely proportional to the sum of both
		//agent's velocities
		float LookAheadTime = ToOffset.Size() /
			(OwnerVehicle->GetMaxSpeed() + ActorVehiclePtr->GetSpeed());

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			WorldOffsetPos + ActorVehiclePtr->Get2DVelocity() * LookAheadTime, FColor(50, 0, 0), false);

		//now Arrive at the predicted future position of the offset
		return Arrive(WorldOffsetPos + ActorVehiclePtr->Get2DVelocity() * LookAheadTime);
	}
	else if (PlayerPawnPtr)
	{
		//Get ship's direction of movement and convert to angle in degrees

		float VelAngle = FMath::RadiansToDegrees(PlayerPawnPtr->Get2DVelocity().GetNormal().GetAngle());

		//rotate offset and cast in world space
		FVector2DPlus WorldOffsetPos = NewOffset.GetRotated(VelAngle) + PlayerPawnPtr->Component2D->GetActorLocation2D();


		FVector2DPlus ToOffset = WorldOffsetPos - OwnerVehicle->Component2D->GetActorLocation2D();

		//the lookahead time is propotional to the distance between the leader
		//and the pursuer; and is inversely proportional to the sum of both
		//agent's velocities
		float LookAheadTime = ToOffset.Size() /
			(OwnerVehicle->GetMaxSpeed() + PlayerPawnPtr->GetSpeed());

		if (OwnerVehicle->IsDrawDebugLines()) OwnerVehicle->PrintDebugLineFromPlayerOrigin(
			WorldOffsetPos + PlayerPawnPtr->Get2DVelocity() * LookAheadTime, FColor(50, 0, 0), false);

		//now Arrive at the predicted future position of the offset
		return Arrive(WorldOffsetPos + PlayerPawnPtr->Get2DVelocity() * LookAheadTime);
	}

	return FVector2D(0.0f, 0.0f);
}

//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::Separation(const TArray<AActorVehicle*>& Neighbors)
{
	FVector2DPlus SteeringForce;
	SteeringForce.Zero();

	for ( int32 a = 0; a<Neighbors.Num(); ++a)
	{
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough. ***also make sure it doesn't
		//include the evade target ***
		if ((Neighbors[a] != OwnerVehicle) && Neighbors[a]->Component2D->IsTagged() /*&&
			(neighbors[a] != m_pTargetAgent1)*/)
		{
			FVector2DPlus ToAgent = OwnerVehicle->Component2D->GetActorLocation2D() - Neighbors[a]->Component2D->GetActorLocation2D();

			//scale the force inversely proportional to the agents distance  
			//from its neighbor.
			SteeringForce += Vec2DNormalize(ToAgent) / ToAgent.Size() * 100;

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(193, .1f, FColor::Green, FString::Printf(TEXT("Pushing Away SteeringForce: %f, %f"), SteeringForce.X, SteeringForce.Y));
			}
		}
	}

	return SteeringForce;
}

//-------------------------------- Cohesion ------------------------------
//
//  returns a steering force that attempts to move the agent towards the
//  center of mass of the agents in its immediate area
//------------------------------------------------------------------------

FVector2DPlus UActorComponentSteeringBehavior::Cohesion(const TArray<AActorVehicle*>& Neighbors)
{
	//first find the center of mass of all the agents
	FVector2DPlus CenterOfMass, SteeringForce;
	CenterOfMass.Zero();
	SteeringForce.Zero();

	int NeighborCount = 0;

	//iterate through the neighbors and sum up all the position vectors
	for (int32 a = 0; a<Neighbors.Num(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined is close enough ***also make sure it doesn't
		//include the evade target ***
		if ((Neighbors[a] != OwnerVehicle) && Neighbors[a]->Component2D->IsTagged() /*&&
			(neighbors[a] != m_pTargetAgent1)*/)
		{
			CenterOfMass += Neighbors[a]->Component2D->GetActorLocation2D();

			++NeighborCount;
		}
	}

	if (NeighborCount > 0)
	{
		//the center of mass is the average of the sum of positions
		CenterOfMass /= (float)NeighborCount;

		//now seek towards that position
		SteeringForce = Seek(CenterOfMass);
	}

	//the magnitude of cohesion is usually much larger than separation or
	//allignment so it usually helps to normalize it.
	return Vec2DNormalize(SteeringForce);
}

//---------------------------- Alignment ---------------------------------
//
//  returns a force that attempts to align this agents heading with that
//  of its neighbors
//------------------------------------------------------------------------
FVector2DPlus UActorComponentSteeringBehavior::Alignment(const TArray<AActorVehicle*>& Neighbors)
{
	//used to record the average heading of the neighbors
	FVector2DPlus AverageHeading;
	AverageHeading.Zero();

	//used to count the number of vehicles in the neighborhood
	int    NeighborCount = 0;

	//iterate through all the tagged vehicles and sum their heading vectors  
	for (int32 a = 0; a<Neighbors.Num(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined  is close enough ***also make sure it doesn't
		//include any evade target ***
		if ((Neighbors[a] != OwnerVehicle) && Neighbors[a]->Component2D->IsTagged() /*&&
			(neighbors[a] != m_pTargetAgent1)*/)
		{
			AverageHeading += Neighbors[a]->GetHeading();

			++NeighborCount;
		}
	}

	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (NeighborCount > 0)
	{
		AverageHeading /= (float)NeighborCount;

		AverageHeading -= OwnerVehicle->GetHeading();
	}

	return AverageHeading;
}
