// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor2D.h"


// Sets default values
AActor2D::AActor2D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Transform2D = FVector2DPlus(GetActorLocation().X, GetActorLocation().Z);

}

//Sets the actor's 2D Location and Places the actor in the 3D scene
bool AActor2D::SetActorLocation2D(const FVector2DPlus & NewLocation)
{
	//Set the X, Y reference
	Transform2D = NewLocation;

	//Move Player in World
	return SetActorLocation(FVector(Transform2D.X, 0.0f, Transform2D.Y), false);


}

// Called when the game starts or when spawned
void AActor2D::BeginPlay()
{
	Super::BeginPlay();
	Transform2D = FVector2DPlus(GetActorLocation().X, GetActorLocation().Z);
}

// Called every frame
void AActor2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

