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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	FVector DisplayGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug;

	Pathfinder2D* Path;

	// Sets default values for this actor's properties
	ADrawGrid2D();

	~ADrawGrid2D() { delete Path; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FColor RandomColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	FVector BoxExtents;
	float ExtentValue;
	
};
