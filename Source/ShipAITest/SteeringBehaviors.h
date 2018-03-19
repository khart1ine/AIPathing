// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Vector2DPlus.h"
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
	WallAvoidance		UMETA(DisplayName = "Wall Avoidance")
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

	/** set binary flags off  **/
	void SeekOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Seek)); }
	void FleeOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Flee)); }
	void ArriveOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Arrive)); }
	void PursuitOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Pursuit)); }
	void EvadeOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Evade)); }
	void WanderOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Wander)); }
	void ObstacleAvoidanceOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::ObstacleAvoidance)); }
	void WallAvoidanceOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::WallAvoidance)); }

	/** Check if binary flag is on **/
	bool IsSeekOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Seek)); }
	bool IsFleeOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Flee)); }
	bool IsArriveOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Arrive)); }
	bool IsPursuitOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Pursuit)); }
	bool IsEvadeOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Evade)); }
	bool IsWanderOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Wander)); }
	bool IsObstacleAvoidanceOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::ObstacleAvoidance)); }
	bool IsWallAvoidanceOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::WallAvoidance)); }

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
	float WanderJitter;
	float DecelerationTweaker; //range between .3 and 1 to slow deceleration for Arrive SB
	float LookAheadPursuit; // how far in front of the player's vector should the vehicle target 

	//Calculates and sums the sterring forces from any active behaviors
	FVector2DPlus CalculateWeightedSum();

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

};

 /** Returns the smaller of 2 floats, couldn't find this in Unreal's math libraries **/
inline float Min(float Vec1, float Vec2)
{
	return Vec1 < Vec2 ? Vec1 : Vec2;
}
