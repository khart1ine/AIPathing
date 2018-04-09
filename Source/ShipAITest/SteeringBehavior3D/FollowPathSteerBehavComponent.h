// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../ActorVehicle3D.h"
#include "FollowPathSteerBehavComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIPAITEST_API UFollowPathSteerBehavComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFollowPathSteerBehavComponent();

	///** Puts the vehicle on a path of waypoints **/
	FVector FollowPath(AActorVehicle3D* OwnerVehicle);

	/** Returns weighted Seek() **/
	FVector FollowPathWeighted(AActorVehicle3D* OwnerVehicle);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteerBehav Weights", meta = (BlueprintProtect = "true"))
	float WeightFollowPath;

	//How close the vehicle needs to be to the waypoint to trigger next waypoint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float WaypointPathDistanceSq;
	
};
