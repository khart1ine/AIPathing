// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WanderSteerBehavComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIPAITEST_API UWanderSteerBehavComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWanderSteerBehavComponent();

	/**  This behavior makes the agent wander about randomly **/
	FVector Wander(class AActorVehicle3D* OwnerVehicle);
	/** Returns weighted Seek() **/
	FVector WanderWeighted(class AActorVehicle3D* OwnerVehicle);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Set bits for weight for wander behavior **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteerBehav Weights", meta = (BlueprintProtect = "true"))
	float WeightWander;

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderJitter;

	/** Edit Wander Steering circle radius to make large turns Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderRadius;

	/** Edit Wander Steering Behavior Settings **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander SB", meta = (BlueprintProtect = "true"))
	float WanderDistance;

private:
	/** Target for Wander behavior **/
	FVector WanderTarget;
		
	
};
