// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Vector2DPlus.h"
#include "ActorVehicle.h"
#include "MovementWalls.h"
#include "MovementGameModeBase.generated.h"

/**
 *
 *	Name:  MovementGameModeBase.h
*
*  Desc:   Manages the 2D Othographic Camera
*
*  Author: Marc Aubanel
* 

*/
USTRUCT(BlueprintType)

/** A Struct that stores the extents of the orthographic camera to know if an object is on screen **/
struct Frustum
{
	GENERATED_BODY()

	Frustum() 
	{
		LeftCameraFrustum = 0.0f;
		RightCameraFrustum = 0.0f;
		TopCameraFrustum = 0.0f;
		BottomCameraFrustum = 0.0f;
	}

		//Letft extent of the screen on X axis
		float LeftCameraFrustum;
	
		//Right extent of the screen on X axis
		float RightCameraFrustum;

		//Top extent of the screen on Z axis
		float TopCameraFrustum;

		//Bottom extent of the screen on Z axis
		float BottomCameraFrustum;
};


UCLASS()
class SHIPAITEST_API AMovementGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	
	// Sets default values for this actor's properties
	AMovementGameModeBase();
	
	/** Contains all references to Obstacles in level **/
	UPROPERTY()
	TArray<class AMovementObstacle *> ObstaclesPtr;

	UPROPERTY()
	TArray<class AActorVehicle *> VehiclesInLevelPtr;

	/** Containts references and for FWallType struct for Walls in level**/
	UPROPERTY()
	TArray<FWallType> WallsPtr;

	/** Returns extent of current camera view **/
	UFUNCTION(BlueprintCallable, Category = "FrustumExtents")
	Frustum GetCameraExtents();

	/** Screen wraps the actdor so they are always in view**/
	UFUNCTION(BlueprintCallable, Category = "FrustumExtents")
	void WrapAround(FVector2DPlus &Pos, float Margin = 0);

	/** Tags whether an object is close enough tocollide with Obstacle**/
	UFUNCTION()
	void TagNeighbors(AActorVehicle* VehiclePtr, const float radius);

	/** Tags which vehicle is in view of neighbor**/
	UFUNCTION()
	void TagAgentNeighbors(AActorVehicle* VehiclePtr, const float radius);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/** Pointer to Camera Actor that gets spawned as fixed camera and not attached to player **/
	class ACameraActor* MainCamera;

	/** Orthographic Camera Width**/
	UPROPERTY(VisibleAnywhere, Category = "FrustumExtents")
	float Width;

	/** Orthographic Camera Width**/
	UPROPERTY(VisibleAnywhere, Category = "FrustumExtents")
	float Height;

	/** CameraComponent pointer to get extents of camera frustum **/
	UPROPERTY()
	class UCameraComponent* CameraComponent;

};

