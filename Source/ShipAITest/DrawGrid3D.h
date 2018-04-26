// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Graph/Pathfinder.h"
#include "DrawGrid3D.generated.h"

UCLASS()
class SHIPAITEST_API ADrawGrid3D : public AActor
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

	// Number of cells deep
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
		int32 GridLimitY;

	// if debug info is shown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
		bool bDrawDebug;

	// Reference to the Pathfinder
	Pathfinder* Pathf;

	// Sets default values for this actor's properties
	ADrawGrid3D();

	~ADrawGrid3D() { delete Pathf; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// Temp vector used for code readibility; used for setting size of debug boxes
	FVector BoxExtents;
	// Temp float for code readibility; used for getting center of grid cells
	float ExtentValue;

};
