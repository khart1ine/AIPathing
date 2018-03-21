// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor2D.h"
#include "Vector2DPlus.h"
#include "PaperSpriteComponent.h"
#include "MovementWalls.generated.h"

USTRUCT(BlueprintType)
struct FWallType
{
	GENERATED_BODY()
	
public:
	FWallType() { LineBegin = FVector2DPlus(); LineEnd =FVector2DPlus(); LineNormal = FVector2DPlus(); WallsRef = nullptr; }
	FWallType(FVector2DPlus LB, FVector2DPlus LE, FVector2DPlus LN,  class AMovementWalls* WR)
	{
		LineBegin = LB;
		LineEnd = LE;
		LineNormal = LN;
		WallsRef = WR;
	}



	/** Begining of line segment **/
	UPROPERTY()
	FVector2DPlus LineBegin;

	/** End of line segment **/
	UPROPERTY()
	FVector2DPlus LineEnd;

	/** Normal of line segment **/
	UPROPERTY()
	FVector2DPlus LineNormal;

	/** Normal of line segment **/
	UPROPERTY()
	class AMovementWalls* WallsRef;

};
/**
 * 
 */
UCLASS()
class SHIPAITEST_API AMovementWalls : public AActor2D
{
	GENERATED_BODY()
	
public:
	/** Set Default Values **/
	AMovementWalls();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Add Sprite to AActor**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (BlueprintProtect = "true"))
		UPaperSpriteComponent * PaperSpriteComponent;

	/** Turn Debug Line Drawing from Actor to Steering Force **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (BlueprintProtect = "true"))
	bool bDrawDebugLines;
	
private:
	/** Reference to room game mode to add instance to TArray  **/
	UPROPERTY()
	class AMovementGameModeBase* GameMode;  

	FVector2DPlus TopLeft;
	FVector2DPlus TopRight;
	FVector2DPlus BottomLeft;
	FVector2DPlus BottomRight;
	FVector2DPlus WallNormalTop;
	FVector2DPlus WallNormalLeft;
	FVector2DPlus WallNormalRight;
	FVector2DPlus WallNormalBottom;

	void PrintDebug(FVector2DPlus Start, FVector2DPlus End);
};
