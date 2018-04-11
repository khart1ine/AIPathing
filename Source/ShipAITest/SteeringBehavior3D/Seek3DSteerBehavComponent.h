// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Seek3DSteerBehavComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIPAITEST_API USeek3DSteerBehavComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USeek3DSteerBehavComponent();

	/* ......................................................................

	BEGIN BEHAVIOR DECLARATIONS

	.......................................................................*/

	/** This behavior moves the agent towards a target position **/
	FVector Seek(FVector TargetPos, class AActorVehicle3D* OwnerVehicle);

	/** Returns weighted Seek() **/
	FVector SeekWeighted(class AActorVehicle3D* OwnerVehicle);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	/** Set bits for weights **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteerBehav Weights", meta = (BlueprintProtect = "true"))
	float WeightSeek;


	
};
