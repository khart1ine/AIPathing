// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorComponentSteeringBehavior3D.h"
#include "ActorVehicle3D.generated.h"

UCLASS()
class SHIPAITEST_API AActorVehicle3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorVehicle3D();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Get's the static non moving target location in world space **/
	UFUNCTION(BlueprintPure, Category = "Target")
	FVector GetSBTargetLocation()const;

	/** Returns the maximum magnitude of the player's allowed velocity **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	inline float GetMaxSpeed()const { return MaxSpeed; }

	/** Gets current 2D velocity vector **/
	UFUNCTION(BlueprintPure, Category = "Movement")
	inline FVector GetVelocity()const { return Velocity; }

	/** Returns the max force which is the most a player can create on their own **/
	UFUNCTION(BlueprintPure, Category = "Movement")
	float GetMaxForce()const { return MaxForce; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/** contains sprite component for player **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (BlueprintProtect = "true"))
	UStaticMeshComponent * StaticMeshComponent;

	/** Static Target set in world **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (BlueprintProtect = "true"))
	AActor* Target;

	/** Vehicle Maximum Speed controls the magnitude of the Velocity**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float MaxSpeed;

	/** The maximum for this entity can produce to power itself **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float MaxForce;

	/** Mass of Actor **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float Mass;

	/** Minimum Velocity agent needs to move **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float MinVelocity;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "2D", meta = (AllowPrivateAccess = "true"))
	UActorComponentSteeringBehavior3D* ComponentSteerBeh3D;

	/** 3D Location of Actor (X, Y) **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector Location;

	//Vehicle velocity
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector Velocity;

	/**a normalized vector pointing in the direction that the actor is heading  **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector Heading;

	/**a normalized vector pointing in the direction that the actor is heading  **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Speed;

	/** Holds the game mode so player can reference camera **/
	//UPROPERTY()
	//class AMovementGameModeBase* GameMode;
	
};
