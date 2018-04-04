// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	int32 GridLimit;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	bool bDrawBoxes;

	ACollisionGrid();

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
