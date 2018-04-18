// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Graph/Pathfinder2D.h"
#include "DrawGrid2D.generated.h"

UCLASS()
class SHIPAITEST_API ADrawGrid2D : public AActor
{
	GENERATED_BODY()
	
public:	

	// Helper vector that keeps track of spatial location of grid points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	FVector DisplayGrid;

	// Cell size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridOffset;

	// Number of horizontal cells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitX;

	// Number of vertical cells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitZ;

	// if debug info is shown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug;

	// Reference to the Pathfinder
	Pathfinder2D* Pathfinder;

	// Sets default values for this actor's properties
	ADrawGrid2D();

	~ADrawGrid2D() { delete Pathfinder; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Helper function that picks a color from FColor at random (not black or transparent)
	FColor RandomColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Temp vector used for code readibility; used for setting size of debug boxes
	FVector BoxExtents;
	// Temp float for code readibility; used for getting center of grid cells
	float ExtentValue;
	
};
