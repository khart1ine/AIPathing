// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FindNearestFace.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIPAITEST_API UFindNearestFace : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFindNearestFace();

	///** Puts the vehicle on a path of waypoints **/
	FVector NearestFace(class AActorVehicle3D* OwnerVehicle, AActor* FaceTarget);

	/** Returns weighted Seek() **/
	FVector NerestFaceWeighted(class AActorVehicle3D* OwnerVehicle, AActor* FaceTarget);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Steering Behavior weight for Finding nearest face behavior **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteerBehav Weights", meta = (BlueprintProtect = "true"))
	float WeightNearestFace;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nearest Face Target", meta = (BlueprintProtect = "true"))
	AActor* ActorTarget;
	
};
