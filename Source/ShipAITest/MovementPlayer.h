// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Actor2D.h"
#include "Vector2DPlus.h"
#include "PaperSpriteComponent.h"
#include "MovementPlayer.generated.h"

/**
*
*	Name:  MovementPlayer.h
*
*  Desc:   Human controlled character with Asteroid like contorls
*
*  Author: Marc Aubanel
*

*/

UCLASS(Blueprintable)
class SHIPAITEST_API AMovementPlayer : public APawn
{
	GENERATED_BODY()

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Sets default values for this pawn's properties
	AMovementPlayer();

	/** Returns the X & Y 2D Value for root location of component in AActor **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE FVector2DPlus GetActorLocation2D() const { return Position; }

	/** Returns the X & Y 2D Actor Forward Vector of AActor **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE FVector2DPlus GetActorForwardVector2D() const { return FVector2DPlus(this->GetActorForwardVector().X, this->GetActorForwardVector().Z); }

	/** Returns the normalized heading of the AActor **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE FVector2DPlus GetHeading() const { return FVector2DPlus::GetVectorFromAngleInDegrees(Pitch); }

	/** Returns the Speed of the Player **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE float GetSpeed() const { return Velocity.Size(); }

	/** Returns the Velocity of the Player **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE FVector2DPlus Get2DVelocity() const { return Velocity; }

	/** Sets the X and Y Value as well as the actual 3D location of AActor **/
	UFUNCTION(BlueprintCallable, Category = "2DLocation")
	bool SetActorLocation2D(const FVector2DPlus & NewLocation);

	/** Returns the X & Y 2D Value for root location of component in AActor **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
		FORCEINLINE float GetRadius() const { return Radius; }


	/** Sets the X and Y FVector 2D Value as well as the actual 3D location of AActor **/
	UFUNCTION(BlueprintCallable, Category = "2DLocation")
		void SetRadius(float R) { Radius = R; };


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (BlueprintProtect = "true"))
	UPaperSpriteComponent * PaperSpriteComponent;

	/** Turn Debug Line Drawing from Actor to Steering Force **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	bool bDrawDebugLines;

	/** Adjusts the radius of the collision volume for Steering Behaviors **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (AllowPrivateAccess = "true"))
	float CollisionRadiusAdjustment;

private:

	/** X Axis for controls in game**/
	UPROPERTY(VisibleAnywhere, Category = "Controls")
	float XInputAxis;

	/** Y Axis for controls in game**/
	UPROPERTY(VisibleAnywhere, Category = "Controls")
	float YInputAxis;


	/**  Stores the 2D location of the player, redundant with FVector 3D**/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	//Returns the 2D (X, Y) location of the Root Component
	FVector2DPlus Transform2D;
	
	/** Player rotation speed in degrees **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Rotation;

	/** Player pitch in degrees **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Pitch;

	/** Player velocity in pixels per second **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Velocity;

	/** Player pitch in degrees **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Position;

	/** Reference to room game mode to access camera extents  **/
	UPROPERTY()
	class AMovementGameModeBase* GameMode;

	/** Holds Radius of shape **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Radius;

	/** Update inputs for horizontal axis**/
	UFUNCTION()
	void MovementRight(float AxisValue);

	/** Update inputs for vertical axis**/
	UFUNCTION()
	void MovementUp(float AxisValue);
	
};

