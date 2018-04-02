// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorComponent2D.h"


// Sets default values for this component's properties
UActorComponent2D::UActorComponent2D()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CollisionRadiusAdjustment = -32.0f;
}


// Called when the game starts
void UActorComponent2D::BeginPlay()
{
	Super::BeginPlay();

	if (Owner)
	{
		Transform2D = FVector2DPlus(Owner->GetActorLocation().X, Owner->GetActorLocation().Z);
	}
	
}


// Called every frame
void UActorComponent2D::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//Sets the actor's 2D Location and Places the actor in the 3D scene
bool UActorComponent2D::SetActorLocation2D(const FVector2DPlus & NewLocation)
{
	//Set the X, Y reference
	Transform2D = NewLocation;

	//Move Player in World
	if (Owner)
	{
		return Owner->SetActorLocation(FVector(Transform2D.X, 0.0f, Transform2D.Y), false);
	}
	else
	{
		return false;
	}


}

