// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Vector2DPlus.h"
#include "MovementWalls.h"
#include "SteeringBehaviors.generated.h"


/**
*
*	Name:  SteeringBehaviors.h
*
*  Desc:   Adapting Programming Game AI By Example to Unreal
*
*  Author: Marc Aubanel for Vehicle based on Programming Game AI By Example
*
*
*/

/** Different steering behaviors **/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class BehaviorTypes : uint8
{
	None 				UMETA(DisplayName = "None"),
	Seek				UMETA(DisplayName = "Seek"),
	Flee				UMETA(DisplayName = "Flee"),
	Arrive				UMETA(DisplayName = "Arrive"),
	Pursuit				UMETA(DisplayName = "Pursuit"),
	Evade				UMETA(DisplayName = "Evade"),
	Wander				UMETA(DisplayName = "Wander"),
	ObstacleAvoidance	UMETA(DisplayName = "Obastacle Avoidance"),
	WallAvoidance		UMETA(DisplayName = "Wall Avoidance"),
	Interpose			UMETA(DisplayName = "Interpose"),
	Hide				UMETA(DisplayName = "Hide"),
	FollowPath			UMETA(DisplayName = "Follow Path"),
	OffsetPursuit		UMETA(Displayname = "Offset Pursuit"),
	Separation			UMETA(DisplayName = "Separation")
};

UCLASS(Blueprintable)
class SHIPAITEST_API USteeringBehaviors : public UObject
{
	GENERATED_BODY()
	
public:
	

	USteeringBehaviors();
	/**  Calculates and sums the sterring forces from any active behaviors **/
	FVector2DPlus Calculate();

	/** Gives this class a reference to the autonomous vehicle **/
	void SetAgent(class AMovementVehicle* NewVehiclePtr);

	/** Setter for thre Wander Steering Behaviors to expose in editor **/
	UFUNCTION(BlueprintPure, Category = "Wander SB")
	FVector2DPlus GetTarget() { return Target; }

	/** Setter foramount of random jitter to add to Wander to expose in editor **/
	UFUNCTION(BlueprintCallable, Category = "Wander SB")
	void SetWanderJitter (float Jitter){WanderJitter = Jitter;}

	/** Setter for Wander radius of circle in front of player **/
	UFUNCTION(BlueprintCallable, Category = "Wander SB")
	void SetWanderRadius(float Radius) { WanderRadius = Radius; }

	/** Distance that circle resides in front of player **/
	UFUNCTION(BlueprintCallable, Category = "Wander SB")
	void SetWanderDistance(float Distance) { WanderDistance = Distance; }

	/** Amount the pursuing or fleeing vehicle looks ahead of the player's movement vector**/
	UFUNCTION(BlueprintCallable, Category = "Pursuit and Evade SB")
	void SetLookAheadPursuit(float LookAhead) { LookAheadPursuit = LookAhead; }

	/** Amount the pursuing or fleeing vehicle looks ahead of the player's movement vector**/
	UFUNCTION(BlueprintCallable, Category = "Pursuit and Evade SB")
	void SetDecelerationTweeker(float Decelerate) { DecelerationTweaker = Decelerate; }

	/** Amount the pursuing or fleeing vehicle looks ahead of the player's movement vector**/
	UFUNCTION(BlueprintCallable, Category = "Pursuit and Evade SB")
	void SetWaypointPathDistance(float DistanceSq) { WaypointPathDistanceSq = DistanceSq; }

	/** Amount of offset for offset pursuit behavior **/
	UFUNCTION(BlueprintCallable, Category = "Offset Pursuit SB")
	void SetOffset(FVector2DPlus Off) { Offset = Off; }

	UFUNCTION(BlueprintCallable, Category = "Weights")
	void SetBehaviorWeights(BehaviorTypes BT, float Amount);

	UFUNCTION(Category = "BehaviorFlag")
	void SetBehaviorFlags(uint32 BF){BehaviorFlags = BF;}

	/** set binary flags on**/
	void SeekOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Seek); }
	void FleeOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Flee); }
	void ArriveOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Arrive); }
	void PursuitOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Pursuit); }
	void EvadeOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Evade); }
	void WanderOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Wander); }
	void ObstacleAvoidanceOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::ObstacleAvoidance); }
	void WallAvoidanceOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::WallAvoidance); }
	void SeparationOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Separation); }
	void InterposeOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Interpose); }
	void HideOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Hide); }
	void FollowPathOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::FollowPath); }
	void OffsetPursuitOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::OffsetPursuit); }

	/** set binary flags off  **/
	void SeekOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Seek)); }
	void FleeOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Flee)); }
	void ArriveOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Arrive)); }
	void PursuitOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Pursuit)); }
	void EvadeOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Evade)); }
	void WanderOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Wander)); }
	void ObstacleAvoidanceOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::ObstacleAvoidance)); }
	void WallAvoidanceOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::WallAvoidance)); }
	void SeparationOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Separation)); }
	void InterposeOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Interpose)); }
	void HideOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Hide)); }
	void FollowPathOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::FollowPath)); }
	void OffsetPursuitOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::OffsetPursuit)); }

	/** Check if binary flag is on **/
	bool IsSeekOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Seek)); }
	bool IsFleeOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Flee)); }
	bool IsArriveOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Arrive)); }
	bool IsPursuitOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Pursuit)); }
	bool IsEvadeOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Evade)); }
	bool IsWanderOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Wander)); }
	bool IsObstacleAvoidanceOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::ObstacleAvoidance)); }
	bool IsWallAvoidanceOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::WallAvoidance)); }
	bool IsSeparationOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Separation)); }
	bool IsInterposeOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Interpose)); }
	bool IsHideOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Hide)); }
	bool IsFollowPathOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::FollowPath)); }
	bool IsOffsetPursuitOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::OffsetPursuit)); }

private:

	/** bitmask for holding which behaviors should be active**/
	uint32 BehaviorFlags;

	//For drawing debug of internal target of Steering Behaviors
	FVector2DPlus SBDebugTarget;
	
	/** Pointer to owner of the Vehicle **/
	class AMovementVehicle* VehiclePtr;

	/** The steering force created by the combined effect of all the selected behaviors **/ 
	FVector2DPlus SteeringForce;

	/** The Current Target **/
	FVector2DPlus Target;

	/** Target for Wander behavior **/ 
	FVector2DPlus WanderTarget;

	/** Size of circle circumpherance that player is wandering around **/
	float WanderRadius;

	/** Distance of circle from player **/
	float WanderDistance;


	//length of the 'detection box' utilized in obstacle avoidance
	float DBoxLength;

	//Multipliers that can be adjusted to affect strength of the 
	//appropriate behavior.  Useful to get flocking the way you require
	//for example

	float WeightSeek;
	float WeightFlee;
	float WeightArrive;
	float WeightPursuit;
	float WeightEvade;
	float WeightWander;
	float WeightObstacleAvoidance;
	float WeightWallAvoidance;
	float WeightSeparation;
	float WeightInterpose;
	float WeightHide;
	float WeightFollowPath;
	float WeightOffsetPursuit;
	float WanderJitter;
	float DecelerationTweaker; //range between .3 and 1 to slow deceleration for Arrive SB
	float LookAheadPursuit; // how far in front of the player's vector should the vehicle target 
	float WaypointPathDistanceSq; //How close the vehicle needs to be to the waypoint to trigger next waypoint

	/** Length of Feelers for wall detector **/
	float WallDetectionFeelerLength;

	//Calculates and sums the sterring forces from any active behaviors
	FVector2DPlus CalculateWeightedSum();

	/** A list of feelers used by vehicle for the wall avoidance behavior**/
	TArray<FVector2DPlus> Feelers;

	//Creates the antenna utilized by the vehicle for the wall avoidance behavior
	void CreateFeelers();

	//Offset used by offset pursuit steering behavior
	FVector2DPlus Offset;

	/* ......................................................................

				BEGIN BEHAVIOR DECLARATIONS

	.......................................................................*/
	
	/** This behavior moves the agent towards a target position **/
	FVector2DPlus Seek(FVector2DPlus TargetPos);

	/** This behavior moves the agent away from a target position **/
	FVector2DPlus Flee(FVector2DPlus TargetPos);

	/** This behavior moves towards a target then comes to a graceful stop **/
	FVector2DPlus Arrive(FVector2DPlus TargetPos);

	/** This behavior moves towards the future position of a moving target if they are not pointing directly at each other **/
	FVector2DPlus Pursuit(class AMovementPlayer* DynamicTarget);

	/**  This does the opposite as pursuit except that it flees from its estimated future position**/
	FVector2DPlus Evade(class AMovementPlayer* DynamicTarget);


	/**  This behavior makes the agent wander about randomly **/
	FVector2DPlus Wander();

	/** this returns a steering force which will attempt to keep the agent 
		away from any obstacles it may encounter **/
	FVector2DPlus ObstacleAvoidance(const TArray<class AMovementObstacle *>& Obstacles);

	/** this retunrs a steering force which will keep the agent away from walls**/
	FVector2DPlus WallAvoidance(const TArray<FWallType> &Walls);

	/**	this results in a steering force that attempts to steer the vehicle
		to the center of the vector connecting two moving agents.**/
		FVector2DPlus Interpose(const AMovementVehicle* VehicleA, const AMovementVehicle* VehicleB);

	/**  Agents looks for a safe hiding place from the player **/
		FVector2DPlus Hide( AMovementPlayer* Hunter, const TArray<class AMovementObstacle*>& Obstacles);

	/** Given the position of a hunter, and the position and radius of
	    an obstacle, this method calculates a position DistanceFromBoundary 
		away from its bounding radius and directly opposite the hunter**/
		FVector2DPlus GetHidingPosition(const FVector2DPlus& PosOb, const float RadiusOb, const FVector2DPlus& PosHunter);

		/** Puts the vehicle on a path of waypoints **/
		FVector2DPlus FollowPath();

		/** this behavior maintains a position, in the direction of offset
			 target vehicle **/
		FVector2DPlus OffsetPursuit(const AMovementPlayer* Agent, const FVector2DPlus NewOffset);

};

 /** Returns the smaller of 2 floats, couldn't find this in Unreal's math libraries **/
inline float Min(float Vec1, float Vec2)
{
	return Vec1 < Vec2 ? Vec1 : Vec2;
}
