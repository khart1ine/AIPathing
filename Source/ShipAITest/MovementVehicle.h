// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor2D.h"
#include "Vector2DPlus.h"
#include "SteeringBehaviors.h"
#include "PaperSpriteComponent.h"
#include "MovementVehicle.generated.h"

/**
*
*	Name:  MovementVehicle.h
*
*  Desc:   Extended version of Unreal's Vector2D with additional 2D functionality
*
*  Author: Marc Aubanel for Vehicle based on Programming Game AI By Example
*
*
*/
UCLASS(Blueprintable)
class SHIPAITEST_API AMovementVehicle : public AActor2D
{
	GENERATED_BODY()

public:

	/**  Set default values **/
	AMovementVehicle();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns the maximum magnitude of the player's allowed velocity **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	inline float GetMaxSpeed()const { return MaxSpeed; }

	/** Returns the maximum magnitude of the player's allowed velocity **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	inline float GetSpeed()const { return Speed; }

	/** Gets current 2D velocity vector **/
	UFUNCTION(BlueprintPure, Category = "Movement")
	inline FVector2DPlus Get2DVelocity()const { return Velocity;}

	/** Gets current 2D velocity vector **/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void VelocityZero() {  Velocity.Zero(); }

	/** Get's the static non moving target location in world space **/
	UFUNCTION(BlueprintPure, Category = "Target")
	FVector2DPlus GetTarget()const;

	/** Returns pointer to the moving player actor **/
	UFUNCTION(BlueprintCallable, Category = "2DLocation")
	AMovementPlayer* ReturnDynamicTargetPtr() { return DynamicTarget; };

	/** Returns the max force which is the most a player can create on their own **/
	UFUNCTION(BlueprintPure, Category = "Movement")
	float GetMaxForce()const { return MaxForce; }

	/** Returns the normalized heading in vehicle **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE FVector2DPlus GetHeading() const { return Heading; }

	/** Returns the time since last frame in milliseconds **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE float GetDeltaTime() const { return DeltaTimeForActor; }

	/** Getting reference to gamemode for SteeringBehaviros**/
	UFUNCTION(BlueprintPure, Category = "Target")
	class AMovementGameModeBase* GetGameMode()const {return GameMode;}
	
	/** Getting reference to gamemode for SteeringBehaviros**/
	UFUNCTION(BlueprintPure, Category = "Interpose")
	class AMovementVehicle* GetInterpose1()const { return Interpose1; }
	
	/** Getting reference to gamemode for SteeringBehaviros**/
	UFUNCTION(BlueprintPure, Category = "Interpose")
		class AMovementVehicle* GetInterpose2()const { return Interpose2; }

	/** Getting reference to gamemode holding obstacle TArray **/
	UFUNCTION(BlueprintCallable, Category = "ObstaclesRef")
	TArray<class AMovementObstacle *>&  GetGameModeObstacles()const;

	/** Getting reference to gamemode holding walls TArray **/
	UFUNCTION(BlueprintCallable, Category = "WallsRef")
	TArray<struct FWallType>&  GetGameModeWalls()const;

	/** Getting reference to gamemode holding walls TArray **/
	UFUNCTION(BlueprintCallable, Category = "WallsRef")
	AMovementPath*  GetMovementPath()const{ return MovementPath; }

	/** Turn Debug Line Drawing from Actor to Steering Force **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	bool bDrawDebugLines;

	/** Getting reference to gamemode holding obstacle TArray **/
	UFUNCTION(BlueprintCallable, Category = "ObstaclesRef")
	void  PrintDebugLineFromPlayerOrigin(FVector2DPlus End, FColor Color, bool Local )const;
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Static Target set in world **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (BlueprintProtect = "true"))
	AActor* StaticTarget;

	/** Dynamic Target set in world that is the player **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (BlueprintProtect = "true"))
	AMovementPlayer* DynamicTarget;

	/** Two targets to test interpose r **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpose", meta = (BlueprintProtect = "true"))
	AMovementVehicle* Interpose1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpose", meta = (BlueprintProtect = "true"))
	AMovementVehicle* Interpose2;


	/** Vehicle Maximum Speed controls the magnitude of the Velocity**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
		float MaxSpeed;

	/** The maximum for this entity can produce to power itself **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
		float MaxForce;

	/** Mass of Actor **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
		float Mass;

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderJitter;

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderRadius;

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderDistance;

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pursuit SB", meta = (BlueprintProtect = "true"))
	float LookAheadPursuit;

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrive SB", meta = (BlueprintProtect = "true"))
	float DecelerationTweaker;

	/** Edit offset for offset pursuit behavior **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrive SB", meta = (BlueprintProtect = "true"))
	FVector2DPlus Offset;

	/** Edit distance to waypoint before moving to next waypoint **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow Path Behavior", meta = (BlueprintProtect = "true"))
	float WaypointPathDistanceSq;

	/** Loops waypoints **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow Path Behavior", meta = (BlueprintProtect = "true"))
	bool bWaypointLoop;

	/** Add Sprite to AActor**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (BlueprintProtect = "true"))
	UPaperSpriteComponent * PaperSpriteComponent;

	/** Stores path for vehicle **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow Path Behavior", meta = (BlueprintProtect = "true"))
	class AMovementPath * MovementPath;

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
	float WeightSeparation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightInterpose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightHide;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightFollowPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightOffsetPursuit;

	

	/** Set bits to turn on and off behaviors **/
	UPROPERTY(EditAnywhere, Category = "Steering Behaviors", meta = (Bitmask, BitmaskEnum = "BehaviorTypes"))
	uint32 BehaviorFlags;

private:

	//Vehicle velocity
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector2DPlus Velocity;

	/** 2D Location of Actor (X, Y) **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Location2D;

	/**a normalized vector pointing in the direction that the actor is heading  **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Heading;

	/**a normalized vector pointing in the direction that the actor is heading  **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Speed;

	/** time in miliseconds from last frame **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float DeltaTimeForActor;

	/*Instance of Steering Behavior for this Actor **/
	UPROPERTY()
	class USteeringBehaviors* Steering;

	/** Holds the game mode so player can reference camera **/
	UPROPERTY()
	class AMovementGameModeBase* GameMode;
	
};
