// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vector2DPlus.h"
#include "Actor2D.generated.h"

/**
*	Base Actor for 2D Steering Behaviors
*/

UCLASS()
class SHIPAITEST_API AActor2D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor2D();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns the X & Y 2D Value for root location of component in AActor **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE FVector2DPlus GetActorLocation2D() const{ return Transform2D;}

	/** Returns the X & Y 2D Actor Forward Vector of AActor **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE FVector2DPlus GetActorForwardVector2D() const { return FVector2DPlus(this->GetActorForwardVector().X, this->GetActorForwardVector().Z); }

	/** Sets the X and Y FVector 2D Value as well as the actual 3D location of AActor **/
	UFUNCTION(BlueprintCallable, Category = "2DLocation")
	bool SetActorLocation2D(const FVector2DPlus & NewLocation);
	
	/** Returns the X & Y 2D Value for root location of component in AActor **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE float GetRadius() const { return Radius; }


	/** Sets the X and Y FVector 2D Value as well as the actual 3D location of AActor **/
	UFUNCTION(BlueprintCallable, Category = "2DLocation")
	void SetRadius(float R) {Radius = R; };


	/** Returns truthiness of AActor tag **/
	UFUNCTION(BlueprintPure, Category = "Tag")
	bool IsTagged()const { return bTag; }

	/** Sets the X and Y FVector 2D Value as well as the actual 3D location of AActor **/
	UFUNCTION(BlueprintCallable, Category = "2DLocation")
	void TagTrue() { bTag = true; }

	/** Sets the X and Y FVector 2D Value as well as the actual 3D location of AActor **/
	UFUNCTION(BlueprintCallable, Category = "2DLocation")
	void TagFalse() { bTag = false; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Holds Radius of shape **/
	float Radius;
	
	/** Adjusts the radius of the collision volume for Steering Behaviors **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (AllowPrivateAccess = "true"))
	float CollisionRadiusAdjustment;


private:
	/** Holds the 2D (X, Y) location of the Root Component, Redundant to 3-D X & Z **/
	FVector2DPlus Transform2D;
	
	/** Generic tag used for collision detection for entities **/
	bool bTag;


	
};
