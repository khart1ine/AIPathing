// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorComponentSteeringBehavior3D.generated.h"

/** Different steering behaviors **/
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class BehaviorTypes3D : uint8
{
	None 				UMETA(DisplayName = "None"),
	Seek				UMETA(DisplayName = "Seek"),
	Arrive				UMETA(DisplayName = "Arrive")

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHIPAITEST_API UActorComponentSteeringBehavior3D : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorComponentSteeringBehavior3D();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**  Calculates and sums the sterring forces from any active behaviors **/
	FVector Calculate();

	/** set binary flags on**/
	void SeekOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes3D::Seek); }
	void ArriveOn() { BehaviorFlags |= 1 << static_cast<uint32>(BehaviorTypes3D::Arrive); }

	/** set binary flags off  **/
	void SeekOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes3D::Seek)); }
	void ArriveOff() { BehaviorFlags &= ~(1 << static_cast<uint32>(BehaviorTypes3D::Arrive)); }

	/** Check if binary flag is on **/
	bool IsSeekOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes3D::Seek)); }
	bool IsArriveOn() { return BehaviorFlags & (1 << static_cast<uint32>(BehaviorTypes3D::Arrive)); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Set bits to turn on and off behaviors **/
	UPROPERTY(EditAnywhere, Category = "Steering Behaviors", meta = (Bitmask, BitmaskEnum = "BehaviorTypes3D"))
	uint32 BehaviorFlags;

	/** Set bits for weights **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightSeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float WeightArrive;

	/** Positive range between to slow deceleration for Arrive SB **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behaviors", meta = (BlueprintProtect = "true"))
	float DecelerationTweaker;


private:

	/** Get actor that owns this component **/
	AActor * Owner;

	/** Pointer to Vehicle That owns this component **/
	class AActorVehicle3D* OwnerVehicle;

	/** The steering force created by the combined effect of all the selected behaviors **/
	FVector SteeringForce;

	//Calculates and sums the sterring forces from any active behaviors
	FVector CalculateWeightedSum();


	/* ......................................................................

	BEGIN BEHAVIOR DECLARATIONS

	.......................................................................*/

	/** This behavior moves the agent towards a target position **/
	FVector Seek(FVector TargetPos);

	///** This behavior moves towards a target then comes to a graceful stop **/
	FVector Arrive(FVector TargetPos);
	
};
