// Fill out your copyright notice in the Description page of Project Settings.

#include "FindNearestFace.h"


// Sets default values for this component's properties
UFindNearestFace::UFindNearestFace()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WeightNearestFace = 1.0f;
}


FVector UFindNearestFace::NearestFace(AActorVehicle3D * OwnerVehicle, AActor * FaceTarget)
{
	return FVector();
}

FVector UFindNearestFace::NerestFaceWeighted(AActorVehicle3D * OwnerVehicle, AActor * FaceTarget)
{
	if (FaceTarget)
	{
		
	}
	return FVector();
}

// Called when the game starts
void UFindNearestFace::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


