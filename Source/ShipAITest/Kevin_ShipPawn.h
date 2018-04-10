// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParticleHelper.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Pawn.h"
#include "Kevin_ShipMovementComponent.h"
#include "Kevin_ShipPawn.generated.h"

UCLASS()
class SHIPAITEST_API AKevin_ShipPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKevin_ShipPawn();

	virtual UKevin_ShipMovementComponent* GetMovementComponent() const override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// LaserPressed and Released change a boolean value for whether the laser should be firing or retracting
	void LaserPressed();
	void LaserReleased();
	void ShootLaser(bool bPressed);

	// DrillPressed and Released change a boolean value for whether the drill should be firing or retracting
	void DrillPressed();
	void DrillReleased();
	void ShootDrill(bool bPressed);

	// Resets VR headset orientation
	void OnResetVR();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = LaserDistance, BlueprintReadOnly)
		float MaxLaserDistance;

	UPROPERTY(EditAnywhere, Category = DrillDistance, BlueprintReadOnly)
		float MaxDrillDistance;

protected:

	/**
	*	This component covers the movement inputs for the ship
	*/
	UPROPERTY(EditAnywhere, Category = Ship, BlueprintReadOnly, meta = (DisplayName = "Ship Movement"))
		class UKevin_ShipMovementComponent *MovementComponent;

	/**
	*	These components handle the ship's actual mesh and the camera for inside the ship
	*/
	UPROPERTY(EditAnywhere, Category = Ship, BlueprintReadOnly, meta = (DisplayName = "Ship Mesh"))
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = Ship, BlueprintReadOnly, meta = (DisplayName = "Camera Anchor", AllowPrivateAccess = "true"))
		class USceneComponent* VRCameraRoot;

	UPROPERTY(EditAnywhere, Category = Ship, BlueprintReadOnly, meta = (DisplayName = "Camera"))
		class UCameraComponent* VRCameraComponent;

	/**
	*	These components cover the laser functionality
	*/
	UPROPERTY(EditAnywhere, Category = ShipLaser, BlueprintReadOnly, meta = (DisplayName = "LaserRoot", AllowPrivateAccess = "true"))
		class USceneComponent* LaserRoot;

	UPROPERTY(EditAnywhere, Category = ShipLaser, BlueprintReadOnly, meta = (DisplayName = "LaserParticles"))
		class UParticleSystemComponent* LaserParticles;

	UPROPERTY(EditAnywhere, Category = ShipLaser, BlueprintReadOnly, meta = (DisplayName = "SparkParticles"))
		class UParticleSystemComponent* SparkParticles;

	UPROPERTY(EditAnywhere, Category = ShipLaser, BlueprintReadOnly, meta = (DisplayName = "StartPoint"))
		class UStaticMeshComponent* LaserStartPoint;

	UPROPERTY(EditAnywhere, Category = ShipLaser, BlueprintReadOnly, meta = (DisplayName = "EndPoint"))
		class UStaticMeshComponent* LaserEndPoint;

	/**
	*	These components cover the drill functionality
	*/
	UPROPERTY(EditAnywhere, Category = ShipDrill, BlueprintReadOnly, meta = (DisplayName = "DrillRoot", AllowPrivateAccess = "true"))
		class USceneComponent* DrillRoot;

	UPROPERTY(EditAnywhere, Category = ShipDrill, BlueprintReadOnly, meta = (DisplayName = "DrillMesh"))
		class UStaticMeshComponent* DrillMesh;

private:
	UInputComponent * PlayerInput;

	bool bLaserPressed;
	bool bDrillPressed;

	FCollisionQueryParams RV_TraceParams;
};
