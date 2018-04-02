// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ActorComponent2D.h"
#include "PaperSpriteComponent.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SHIPAITEST_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "2D", meta = (AllowPrivateAccess = "true"))
	UActorComponent2D* Component2D;

	/** Gets current 2D velocity vector **/
	UFUNCTION(BlueprintPure, Category = "Movement")
	inline FVector2DPlus Get2DVelocity()const { return Velocity; }

	/** Returns the maximum magnitude of the player's allowed velocity **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	inline float GetSpeed()const { return Velocity.Size(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** contains sprite component for player **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (BlueprintProtect = "true"))
	UPaperSpriteComponent * PaperSpriteComponent;

	/** Turn speed in degrees per second **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float MaxTurnSpeed;

	/** The maximum speed this entity may travel at **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float MaxPlayerSpeed;

	/** The maximum force this entity can produce to power itself **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float Force;

	/** The maximum for this entity can produce to power itself **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float Friction;

	/** Turn Debug Line Drawing from Actor to Steering Force **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	bool bDrawDebugLines;

private:

	/** X Axis for controls in game**/
	UPROPERTY(VisibleAnywhere, Category = "Controls")
	float XInputAxis;

	/** Y Axis for controls in game**/
	UPROPERTY(VisibleAnywhere, Category = "Controls")
	float YInputAxis;

	/** Player pitch in degrees **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Location2D;

	/** Player rotation speed in degrees **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Rotation;

	/** Player pitch in degrees **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Pitch;

	/** Player velocity in pixels per second **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Velocity;

	/** Reference to room game mode to access camera extents  **/
	UPROPERTY()
	class AMovementGameModeBase* GameMode;

	/** Update inputs for horizontal axis**/
	UFUNCTION()
	void MovementRight(float AxisValue);

	/** Update inputs for vertical axis**/
	UFUNCTION()
	void MovementUp(float AxisValue);
	
};
