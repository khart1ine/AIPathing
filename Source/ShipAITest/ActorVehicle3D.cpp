// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorVehicle3D.h"
#include "Engine.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values
AActorVehicle3D::AActorVehicle3D()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Setup Mesh SubObject
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vehicle Mesh"));
	RootComponent = StaticMeshComponent;

	ComponentSteerBeh3D = CreateDefaultSubobject<UActorComponentSteeringBehavior3D>(TEXT("Sterring Behavior 3D Component"));
	SeekSteerBehav = CreateDefaultSubobject<USeek3DSteerBehavComponent>(TEXT("Seek Steering Behavior"));
	ArriveSteerBehav = CreateDefaultSubobject<UArrive3DSteerBehavComponent>(TEXT("Arrive Steering Behavior"));
	FollowPathSteerBehav = CreateDefaultSubobject<UFollowPathSteerBehavComponent>(TEXT("Follow Path Steering Behavior"));
	WanderPathSteerBehav = CreateDefaultSubobject<UWanderSteerBehavComponent>(TEXT("Wander Path Steering Behavior"));

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
	
	int count = 10;
	TArray <const int*> temp = { &count };
	while (temp.Num() != 0)
	{
		const int* Next = temp.Top();

		GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("temp[Num()-1]: %d"),
			*temp[temp.Num() - 1]));

		temp.Pop();
		if (count == 10)
		{
			temp.Emplace(&count);
			temp.Emplace(&count);
		}
		if (count > 0)
		{
			temp.Emplace(&count);
			--count;
		}
	}
}

// Called every frame
void AActorVehicle3D::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//Delta Time requied by outside function
	DeltaTimeForActor = DeltaTime;

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

	MinVelocity = 10.0f;

//	if (GEngine)
//	{
//GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("Speed: %f"), Speed));
//	//	GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Green, FString::Printf(TEXT("SteeringForce: %f, %f, %f"), SteeringForce.X, SteeringForce.Y, SteeringForce.Z));
//		//GEngine->AddOnScreenDebugMessage(3, 15.0f, FColor::Green, FString::Printf(TEXT("Velocity is: %f"), Velocity.Size()));
//	}

	//Move ship
	if (Speed > MinVelocity)
	{
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
	else
	{
		Velocity = FVector::ZeroVector;
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

