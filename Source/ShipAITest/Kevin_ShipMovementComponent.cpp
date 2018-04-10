// Fill out your copyright notice in the Description page of Project Settings.

#include "Kevin_ShipMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Public/EngineGlobals.h"
#include "Engine.h"

UKevin_ShipMovementComponent::UKevin_ShipMovementComponent()
{
	MaxAcceleration = 1000.0f;
	TurnSpeed = 90.0f;
	AccelerationDamping = 0.65f;
	VelocityDamping = 0.95f;
	Acceleration = FVector::ZeroVector;
	CurrentPitch = 0.0f;
	CurrentYaw = 0.0f;
	CurrentRoll = 0.0f;
	PitchInversionMod = 1.0f;
	BackwardsModifier = 0.2f;
	ForwardsModifier = 1.0f;
	StrafeModifier = 1.0f;
	YawModifier = 1.0f;
	RollModifier = 0.8f;
	PitchModifier = 1.0f;
}

void UKevin_ShipMovementComponent::MoveForward(float AxisValue)
{
	if (PawnOwner && UpdatedComponent == PawnOwner->GetRootComponent())
	{
		if (AxisValue < 0)
		{
			AddInputVector(PawnOwner->GetActorForwardVector() * AxisValue * BackwardsModifier);
		}
		else
		{
			AddInputVector(PawnOwner->GetActorForwardVector() * AxisValue * ForwardsModifier);
		}
	}
}

void UKevin_ShipMovementComponent::SlideRight(float AxisValue)
{
	if (PawnOwner && UpdatedComponent == PawnOwner->GetRootComponent())
	{
		AddInputVector(PawnOwner->GetActorRightVector() * AxisValue);
	}
}

void UKevin_ShipMovementComponent::SlideUp(float AxisValue)
{
	if (PawnOwner && UpdatedComponent == PawnOwner->GetRootComponent())
	{
		AddInputVector(PawnOwner->GetActorUpVector() * AxisValue);
	}
}

void UKevin_ShipMovementComponent::Yaw(float AxisValue)
{
	float TargetYaw = (AxisValue * TurnSpeed);

	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void UKevin_ShipMovementComponent::Pitch(float AxisValue)
{
	float TargetPitch = (AxisValue * TurnSpeed) * PitchInversionMod;

	CurrentPitch = FMath::FInterpTo(CurrentPitch, TargetPitch, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void UKevin_ShipMovementComponent::Roll(float AxisValue)
{
	float TargetRoll = (AxisValue * TurnSpeed);

	CurrentRoll = FMath::FInterpTo(CurrentRoll, TargetRoll, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void UKevin_ShipMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	//Calculate change in rotation this frame
	FRotator CurrentRotation = UpdatedComponent->GetComponentRotation();
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitch * DeltaTime;
	DeltaRotation.Yaw = CurrentYaw * DeltaTime;
	DeltaRotation.Roll = CurrentRoll * DeltaTime;

	UpdatedComponent->AddLocalRotation(DeltaRotation);
	PawnOwner->AddControllerPitchInput(DeltaRotation.Pitch);
	PawnOwner->AddControllerYawInput(DeltaRotation.Yaw);
	PawnOwner->AddControllerRollInput(DeltaRotation.Roll);

	//Get and clear the movement vector that we set using AddInputVector
	const FVector InputVector = ConsumeInputVector().GetClampedToMaxSize(1.0f);
	Acceleration += InputVector * MaxAcceleration;
	Acceleration = Acceleration.GetClampedToMaxSize(MaxAcceleration);

	//Update velocity and displacement
	Velocity += Acceleration * DeltaTime;
	FVector DesiredMovementThisFrame = Velocity * DeltaTime;

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		CurrentRotation = UpdatedComponent->GetComponentRotation();
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, CurrentRotation, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}

	//Add some damping so ship come to a halt if there is no input
	Acceleration *= AccelerationDamping;
	Velocity *= VelocityDamping;
}

void UKevin_ShipMovementComponent::InvertPitch()
{
	PitchInversionMod = -PitchInversionMod;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Yellow, TEXT("Modifier Changed"));

}


