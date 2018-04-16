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

	/** Location of the Camera **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Position", meta = (BlueprintProtect = "true"))
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Position", meta = (BlueprintProtect = "true"))
	FRotator Rotation;

private:

	/** Pointer to Camera Actor that gets spawned as fixed camera and not attached to player **/
	class ACameraActor* MainCamera;
	
	/** CameraComponent pointer to get extents of camera frustum **/
	UPROPERTY()
	class UCameraComponent* CameraComponent;




};
