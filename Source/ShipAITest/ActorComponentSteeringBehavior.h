// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vector2DPlus.h"
#include "MovementWalls.h"
#include "PlayerPawn.h"
#include "ActorComponentSteeringBehavior.generated.h"

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
	ObstacleAvoidance	UMETA(DisplayName = "Obstacle Avoidance"),
	WallAvoidance		UMETA(DisplayName = "Wall Avoidance"),
	Interpose			UMETA(DisplayName = "Interpose"),
	Hide				UMETA(DisplayName = "Hide"),
	FollowPath			UMETA(DisplayName = "Follow Path"),
	OffsetPursuit		UMETA(Displayname = "Offset Pursuit"),
	Separation			UMETA(DisplayName = "Separation"),
	Alignment			UMETA(DisplayName = "Alignment"),
	Cohesion			UMETA(DisplayName = "Cohesion")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIPAITEST_API UActorComponentSteeringBehavior : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorComponentSteeringBehavior();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**  Calculates and sums the sterring forces from any active behaviors **/
	FVector2DPlus Calculate();

	//Calculates and sums the sterring forces from any active behaviors
	FVector2DPlus CalculateWeightedSum();

	/** Amount of offset for offset pursuit behavior **/
	UFUNCTION(BlueprintCallable, Category = "Offset Pursuit SB")
	void SetOffset(FVector2DPlus Off) { Offset = Off; }

	/** set binary flags on**/
	void SeekOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Seek); }
	void FleeOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Flee); }
	void ArriveOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Arrive); }
	void PursuitOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Pursuit); }
	void EvadeOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Evade); }
	void WanderOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Wander); }
	void ObstacleAvoidanceOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::ObstacleAvoidance); }
	void WallAvoidanceOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::WallAvoidance); }
	void InterposeOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Interpose); }
	void HideOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Hide); }
	void FollowPathOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::FollowPath); }
	void OffsetPursuitOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::OffsetPursuit); }
	void SeparationOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Separation); }
	void AlignmentOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Alignment); }
	void CohesionOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes::Cohesion); }

	/** set binary flags off  **/
	void SeekOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Seek)); }
	void FleeOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Flee)); }
	void ArriveOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Arrive)); }
	void PursuitOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Pursuit)); }
	void EvadeOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Evade)); }
	void WanderOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Wander)); }
	void ObstacleAvoidanceOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::ObstacleAvoidance)); }
	void WallAvoidanceOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::WallAvoidance)); }
	void InterposeOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Interpose)); }
	void HideOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Hide)); }
	void FollowPathOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::FollowPath)); }
	void OffsetPursuitOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::OffsetPursuit)); }
	void SeparationOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Separation)); }
	void AlignmentOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Alignment)); }
	void CohesionOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes::Cohesion)); }

	/** Check if binary flag is on **/
	bool IsSeekOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Seek)); }
	bool IsFleeOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Flee)); }
	bool IsArriveOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Arrive)); }
	bool IsPursuitOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Pursuit)); }
	bool IsEvadeOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Evade)); }
	bool IsWanderOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Wander)); }
	bool IsObstacleAvoidanceOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::ObstacleAvoidance)); }
	bool IsWallAvoidanceOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::WallAvoidance)); }
	bool IsInterposeOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Interpose)); }
	bool IsHideOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Hide)); }
	bool IsFollowPathOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::FollowPath)); }
	bool IsOffsetPursuitOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::OffsetPursuit)); }
	bool IsSeparationOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Separation)); }
	bool IsAlignmentOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Alignment)); }
	bool IsCohesionOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes::Cohesion)); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Set bits to turn on and off behaviors **/
	UPROPERTY(EditAnywhere, Category = "Steering Behaviors", meta = (Bitmask, BitmaskEnum = "BehaviorTypes"))
	uint32 BehaviorFlags;

	/** Set bits for weights **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightSeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightFlee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightArrive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightPursuit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightEvade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightWander;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightObstacleAvoidance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightWallAvoidance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightInterpose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightHide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightFollowPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightOffsetPursuit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightSeparation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightAlignment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightCohesion;
	
	/** Positive range between to slow deceleration for Arrive SB **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float DecelerationTweaker; 

	/** how far in front of the player's vector should the vehicle target for Pursuit and & Evade SB **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float LookAheadPursuit; 

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderJitter;

	/** Edit Wander Steering circle radius to make large turns Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderRadius;

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderDistance;

	/** Minimum Length for Detection Box **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float MinDBoxLength;

	/** Length of Feelers for wall detector **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float WallDetectionFeelerLength;

	//How close the vehicle needs to be to the waypoint to trigger next waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float WaypointPathDistanceSq; 


		
private:
	/** The steering force created by the combined effect of all the selected behaviors **/
	FVector2DPlus SteeringForce;

	/** Length of box in front of player to check for obstacles for Obstacle Avoidance SB **/
	float DBoxLength;

	/** Get actor that owns this component **/
	AActor* Owner;

	class AActorVehicle* OwnerVehicle;

	/** A list of feelers used by vehicle for the wall avoidance behavior**/
	TArray<FVector2DPlus> Feelers;

	/** Target for Wander behavior **/
	FVector2DPlus WanderTarget;

	/** Stores normalized jitter value for debug lines **/
	FVector2DPlus DebugTarget;

	//Offset used by offset pursuit steering behavior
	FVector2DPlus Offset;

	/* ......................................................................

	BEGIN BEHAVIOR DECLARATIONS

	.......................................................................*/

	/** This behavior moves the agent towards a target position **/
	FVector2DPlus Seek(FVector2DPlus TargetPos);

	/** This behavior moves the agent away from a target position **/
	FVector2DPlus Flee(FVector2DPlus TargetPos);

	///** This behavior moves towards a target then comes to a graceful stop **/
	FVector2DPlus Arrive(FVector2DPlus TargetPos);

	///** This behavior moves towards the future position of a moving target if they are not pointing directly at each other **/
	FVector2DPlus Pursuit(class AActor* TargetPos, bool IsNPC);

	///**  This does the opposite as pursuit except that it flees from its estimated future position**/
	FVector2DPlus Evade(class AActor* TargetPos, bool IsNPC);


	///**  This behavior makes the agent wander about randomly **/
	FVector2DPlus Wander();

	///** this returns a steering force which will attempt to keep the agent
	//away from any obstacles it may encounter **/
	FVector2DPlus ObstacleAvoidance(const TArray<class AMovementObstacle *>& Obstacles);

	///** this retunrs a steering force which will keep the agent away from walls**/
	FVector2DPlus WallAvoidance(const TArray<FWallType> &Walls);

	//Creates the antenna utilized by the vehicle for the wall avoidance behavior
	void CreateFeelers();

	///**	this results in a steering force that attempts to steer the vehicle
	//to the center of the vector connecting two moving agents.**/
	FVector2DPlus Interpose(const AActorVehicle* VehicleA, const AActorVehicle* VehicleB);

	///**  Agents looks for a safe hiding place from the player **/
	FVector2DPlus Hide(APlayerPawn* Hunter, const TArray<class AMovementObstacle*>& Obstacles);

	///** Given the position of a hunter, and the position and radius of
	//an obstacle, this method calculates a position DistanceFromBoundary
	//away from its bounding radius and directly opposite the hunter**/
	FVector2DPlus GetHidingPosition(const FVector2DPlus& PosOb, const float RadiusOb, const FVector2DPlus& PosHunter);

	///** Puts the vehicle on a path of waypoints **/
	FVector2DPlus FollowPath();

	///** this behavior maintains a position, in the direction of offset
	//target vehicle **/
	FVector2DPlus OffsetPursuit( AActor* Agent, const FVector2DPlus NewOffset);

	// -- Group Behaviors -- //

	FVector2DPlus Separation(const TArray<AActorVehicle*> &Neighbors);

	FVector2DPlus Cohesion(const TArray<AActorVehicle*> &Neighbors);

	FVector2DPlus Alignment(const TArray<AActorVehicle*> &Neighbors);
};


/** Returns the smaller of 2 floats, couldn't find this in Unreal's math libraries **/
inline float Min(float Vec1, float Vec2)
{
	return Vec1 < Vec2 ? Vec1 : Vec2;
}