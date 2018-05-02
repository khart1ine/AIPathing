// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Graph/Pathfinder.h"
#include "ActorVehicle3D.h"
#include "Engine/TargetPoint.h"
#include "SteeringBehavior3D/MovementPath3D.h"
#include "CollisionGrid.generated.h"

/**
 * 
 */
UCLASS()
class SHIPAITEST_API ACollisionGrid : public AActor
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	FVector DebugGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	float GridOffset;	

	// Number of horizontal cells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitX;

	// Number of vertical cells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitZ;

	// Number of cells deep
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIShip", meta = (AllowPrivateAccess = "true"))
	AActorVehicle3D* AIShip;

	AMovementPath3D* MPPath;

	TArray<ATargetPoint*> WaypointList;

	Pathfinder* Pathing;

	ACollisionGrid();

	~ACollisionGrid() { delete Pathing; }

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

private:

	// False values for IsValid mean space is empty and box should be green; 
	// True values for IsValid mean space is occupied and box should be red
	TArray<FBox> CollisionGrid;

	FCollisionShape CollisionBox;
	FVector BoxExtents;
	float ExtentValue;
};
