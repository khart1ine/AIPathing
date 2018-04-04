// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawGrid2D.generated.h"

UCLASS()
class SHIPAITEST_API ADrawGrid2D : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	FVector DisplayGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	float GridOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimitZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug;

	// Sets default values for this actor's properties
	ADrawGrid2D();

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
