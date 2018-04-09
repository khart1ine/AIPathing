// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MovementGameModeBase3D.generated.h"

/**
 * 
 */
UCLASS()
class SHIPAITEST_API AMovementGameModeBase3D : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMovementGameModeBase3D();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/** Pointer to Camera Actor that gets spawned as fixed camera and not attached to player **/
	class ACameraActor* MainCamera;
	
	/** CameraComponent pointer to get extents of camera frustum **/
	UPROPERTY()
	class UCameraComponent* CameraComponent;

	/** Location of the Camera **/
	FVector Location;

	FRotator Rotation;


};
