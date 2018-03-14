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

	//Inherits GetActorLocation2D() rom parent

	/** Returns the maximum magnitude of the player's allowed velocity **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	inline float GetMaxSpeed()const { return MaxSpeed; }

	/** Gets current 2D velocity vector **/
	UFUNCTION(BlueprintPure, Category = "Movement")
	inline FVector2DPlus Get2DVelocity()const { return Velocity;}

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
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Static Target set in world **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (BlueprintProtect = "true"))
	AActor* StaticTarget;

	/** Dynamic Target set in world that is the player **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (BlueprintProtect = "true"))
	AMovementPlayer* DynamicTarget;
	
	/** Change Behavior **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (BlueprintProtect = "true"))
	BehaviorTypes Behavior;

	/** Vehicle Maximum Speed controls the magnitude of the Velocity**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
		float MaxSpeed;

	/** The maximum for this entity can produce to power itself **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
		float MaxForce;

	/** Mass of Actor **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
		float Mass;

	/** Turn Debug Line Drawing from Actor to Steering Force **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	bool bDrawDebugLines;

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

	/** Add Sprite to AActor**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (BlueprintProtect = "true"))
	UPaperSpriteComponent * PaperSpriteComponent;
	
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