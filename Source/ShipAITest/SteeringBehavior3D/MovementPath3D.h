// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"
#include "MovementPath3D.generated.h"

UCLASS()
class SHIPAITEST_API AMovementPath3D : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovementPath3D();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//returns the current waypoint
	FVector    CurrentWaypoint()const { return FVector((*CurrWaypointIt)->GetActorLocation()); }

	ATargetPoint* GetCurrWaypointTarget() const { return *CurrWaypointIt; }

	//returns true if the end of the list has been reached
	bool Finished() { return !(*CurrWaypointIt != TargetPoints.Last()); }

	//moves the iterator on to the next waypoint in the list
	inline bool SetNextWaypoint();

	void SetPathLoop(bool isLooped) { bPathLooped = isLooped; }

	bool GetPathLooped() const { return bPathLooped; }

	void SetTargetPoints(TArray<ATargetPoint*> Targets) { TargetPoints = Targets; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints", meta = (BlueprintProtect = "true"))
	TArray<ATargetPoint*> TargetPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints", meta = (BlueprintProtect = "true"))
		bool bDrawDebugLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waypoints", meta = (BlueprintProtect = "true"))
		bool bPathLooped;

private:
	TArray<ATargetPoint*>::TConstIterator CurrWaypointIt = TargetPoints.CreateConstIterator();


	
	
};

inline bool AMovementPath3D::SetNextWaypoint()
{
	check(TargetPoints.Num() > 0);

	if (!++CurrWaypointIt)
	{
		if (bPathLooped)
		{
			CurrWaypointIt.Reset();
			return true;
		}
		else
		{
			--CurrWaypointIt;
			return false;
		}
	}
	return true;

}