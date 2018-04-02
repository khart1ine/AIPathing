// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TargetPoint.h"
#include "Vector2DPlus.h"
#include "MovementPath.generated.h"

//------------------------------------------------------------------------
//
//  Name:   MovementPath.h
//
//  Desc:   class to define, manage, and traverse a path (defined by a series of UE TargetPoint(s))
//          
//
//  Adapted from: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

UCLASS()
class SHIPAITEST_API AMovementPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovementPath();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//returns the current waypoint
	FVector2DPlus    CurrentWaypoint()const { return FVector2DPlus((*CurrWaypointIt)->GetActorLocation().X, (*CurrWaypointIt)->GetActorLocation().Z); }

	ATargetPoint* GetCurrWaypointTarget() const {return *CurrWaypointIt;}

	//returns true if the end of the list has been reached
	bool Finished() { return !(*CurrWaypointIt !=TargetPoints.Last()); }

	//moves the iterator on to the next waypoint in the list
	inline bool SetNextWaypoint();

	void SetPathLoop(bool isLooped) { bPathLooped = isLooped; }

	bool GetPathLooped() const { return bPathLooped; }


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
	TArray<ATargetPoint*>::TConstIterator CurrWaypointIt =  TargetPoints.CreateConstIterator();




	
};

//-------------------- Methods -------------------------------------------

inline bool AMovementPath::SetNextWaypoint()
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
