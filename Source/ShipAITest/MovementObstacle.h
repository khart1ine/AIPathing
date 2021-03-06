// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "ActorComponent2D.h"
#include "MovementObstacle.generated.h"

/**
 * 
 */
UCLASS()
class SHIPAITEST_API AMovementObstacle : public AActor
{
	GENERATED_BODY()

public:
	/** Set Default Values **/
	AMovementObstacle();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Getting reference to gamemode holding obstacle TArray **/
	UFUNCTION(BlueprintCallable, Category = "ObstaclesRef")
	UActorComponent2D* GetComponent2D()const {return Component2D;}
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Add Sprite to AActor**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (BlueprintProtect = "true"))
	UPaperSpriteComponent * PaperSpriteComponent;

	/** Turn Debug Line Drawing from Actor to Steering Force **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	bool bDrawDebugLines;

private:
	/** Reference to room game mode to add instance to TArray  **/
	UPROPERTY()
	class AMovementGameModeBase* GameMode;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "2D", meta = (AllowPrivateAccess = "true"))
	UActorComponent2D* Component2D;
	
};
