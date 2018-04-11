// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Arrive3DSteerBehavComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIPAITEST_API UArrive3DSteerBehavComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArrive3DSteerBehavComponent();

	///** This behavior moves towards a target then comes to a graceful stop **/
	FVector Arrive(class AActorVehicle3D* OwnerVehicle, FVector TargetPos);
	
	/** Returns weighted Seek() **/
	FVector ArriveWeighted(class AActorVehicle3D* OwnerVehicle);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteerBehav Weights", meta = (BlueprintProtect = "true"))
	float WeightArrive;

	/** Positive range between to slow deceleration for Arrive SB **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float DecelerationTweaker;
	
};
