// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kevin_ShipMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHIPAITEST_API UKevin_ShipMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UKevin_ShipMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunciton);

	void MoveForward(float AxisValue);
	void SlideRight(float AxisValue);
	void SlideUp(float AxisValue);
	void Yaw(float AxisValue);
	void Pitch(float AxisValue);
	void Roll(float AxisValue);

	void InvertPitch();


	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float MaxAcceleration;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float TurnSpeed;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float AccelerationDamping;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float VelocityDamping;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float BackwardsModifier;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float ForwardsModifier;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float StrafeModifier;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float PitchModifier;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float YawModifier;

	UPROPERTY(EditAnywhere, Category = ShipMovement, BlueprintReadOnly)
		float RollModifier;

protected:
	FVector Acceleration;
	float CurrentPitch;
	float CurrentYaw;
	float CurrentRoll;
	float PitchInversionMod;
	
	
};
