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
	FVector Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	float GridOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GridProperty", meta = (AllowPrivateAccess = "true"))
	float GridLimit;	

	ACollisionGrid();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

};
