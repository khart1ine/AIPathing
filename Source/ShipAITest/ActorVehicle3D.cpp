// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorVehicle3D.h"


// Sets default values
AActorVehicle3D::AActorVehicle3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Setup Sprite SubObject
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vehicle Mesh"));
	RootComponent = StaticMeshComponent;

	ComponentSteerBeh3D = CreateDefaultSubobject<UActorComponentSteeringBehavior3D>(TEXT("Sterring Behavior 3D Component"));

	//Initializes default variables
	MaxSpeed = 180.0f;
	MaxForce = 120.0f;
	Mass = 1.0f;
	Velocity = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AActorVehicle3D::BeginPlay()
{
	Super::BeginPlay();

	Location = GetActorLocation();
	
}

// Called every frame
void AActorVehicle3D::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//Stores the upcoming force required by the vehicle
	FVector SteeringForce;

	//Calculate the combined force of the steering behavior
	SteeringForce = ComponentSteerBeh3D->Calculate();

	//Acceleratoin = Force/Mass
	FVector Acceleration = SteeringForce / Mass;

	//Update velocity accounting for time since last frame
	Velocity += Acceleration * DeltaTime;

	//Make sure velocity does not exceed max speed
	Velocity = Velocity.GetClampedToMaxSize(GetMaxSpeed());

	//Update location accounting for time since last from last frame
	Location += Velocity * DeltaTime;

	//Sets Speed so we can see it in Unreal Editor, can be removed if used in game as it does nothing
	Speed = Velocity.Size();

	if (GEngine)
	{
//GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("Location: %f, %f, %f"), Location.X, Location.Y, Location.Z));
	//	GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Green, FString::Printf(TEXT("SteeringForce: %f, %f, %f"), SteeringForce.X, SteeringForce.Y, SteeringForce.Z));
		//GEngine->AddOnScreenDebugMessage(3, 15.0f, FColor::Green, FString::Printf(TEXT("Velocity is: %f"), Velocity.Size()));
	}

	//Updates Location of actor
	SetActorLocation(Location);

	//Update heading if the actor is moving
	if (Velocity.SizeSquared() > .000000001)
	{
		//Normalize velocity to get heading
		Heading = Velocity.GetSafeNormal();

		//Grab rotation from actor
		FRotator ActorRotation = Heading.Rotation();

		//Update angles.  Make sure you include Yaw and Roll otherwise they change a bit causing visual glitches.
		//In this game we are only rotating around the Y axis.
	/*	ActorRotation.Pitch = FMath::RadiansToDegrees(FMath::Atan2(Heading.Y, Heading.X));
		ActorRotation.Yaw = 0.f;
		ActorRotation.Roll = 0.f;*/

		//Actually rotate actor to new angle
		SetActorRotation(ActorRotation);

	}
}

FVector AActorVehicle3D::GetSBTargetLocation() const
{
	if (Target != nullptr)
	{
		return Target->GetActorLocation();
	}
	else
	{
		return FVector::ZeroVector;
	}
}

