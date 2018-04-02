// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorVehicle.h"
#include "MovementGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "PaperSprite.h"


// Sets default values
AActorVehicle::AActorVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Sprite SubObject
	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Player Sprite"));
	RootComponent = PaperSpriteComponent;

	ComponentSteerBeh2D = CreateDefaultSubobject<UActorComponentSteeringBehavior>(TEXT("Sterring Behabior Component"));

	Component2D = CreateDefaultSubobject<UActorComponent2D>(TEXT("2D Actor Component"));

	//Initializes default variables
	MaxSpeed = 180.0f;
	Mass = 1.0f;
	MaxForce = 120.0f;


	Velocity = FVector2DPlus(0.0f, 0.0f);
}

FVector2DPlus AActorVehicle::GetSBTargetLocation() const
{
	if (Target != nullptr)
	{
		return FVector2DPlus((double)Target->GetActorLocation().X, (double)Target->GetActorLocation().Z);
	}
	else
	{
		return FVector2DPlus(0.0, 0.0);
	}
}

void AActorVehicle::PrintDebugLineFromPlayerOrigin(FVector2DPlus End, FColor Color, bool Local) const
{
	//FVector Start = (GetActorForwardVector() * Radius) + GetActorLocation();
	FVector Start = GetActorLocation();
	FVector Finish;

	(Local) ? Finish = FVector(Start.X + End.X, 0.0F, Start.Z + End.Y) : Finish = FVector(End.X, 0.0f, End.Y);

	End *= DeltaTimeForActor * 10.0f;
	DrawDebugLine(
		GetWorld(),
		Start,
		Finish,
		Color,
		false,  //not persistent ( goes away)
		-1,
		0,
		2	//thickness
	);
}

TArray<class AMovementObstacle*>& AActorVehicle::GetGameModeObstacles() const
{
	return GameMode->ObstaclesPtr;
}

TArray<struct FWallType>& AActorVehicle::GetGameModeWalls() const
{
	return GameMode->WallsPtr;
}

TArray<AActorVehicle*>& AActorVehicle::GetAgentsInLevel() const
{
	return GameMode->VehiclesInLevelPtr;
}

// Called when the game starts or when spawned
void AActorVehicle::BeginPlay()
{
	Super::BeginPlay();

	//Game Game mode base so we can access camera
	AGameModeBase* GMB = UGameplayStatics::GetGameMode(this);

	if (GMB)
	{
		GameMode = (AMovementGameModeBase *)GMB;
		GameMode->VehiclesInLevelPtr.AddUnique(this);
	}

	//Puts current location in editor into the Location for the 2D Struct
	Location2D = FVector2DPlus(0.0f, 0.0f);
	Location2D.X = GetActorLocation().X;
	Location2D.Y = GetActorLocation().Z;

	//Get Actor Radius
	UPaperSprite* PaperSprite = PaperSpriteComponent->GetSprite();

	if (PaperSprite)
	{
		Component2D->SetRadius (FVector2DPlus::Diagonal(PaperSprite->GetSourceSize().X, PaperSprite->GetSourceSize().Y, Component2D->GetCollisionRadiusAdjustment()) / 2);
	}

	if (ComponentSteerBeh2D->IsOffsetPursuitOn())
	{
		/** Sets position relative to leader for offset pursuit**/
		FVector LocalPos = Target->GetTransform().InverseTransformPosition(GetActorLocation());
		FVector2D Offset = FVector2DPlus(LocalPos.X, LocalPos.Z);
		ComponentSteerBeh2D->SetOffset(Offset);
	}
	
}

// Called every frame
void AActorVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Delta Time requied by outside function
	DeltaTimeForActor = DeltaTime;

	//Stores the upcoming force required by the vehicle
	FVector2DPlus SteeringForce;

	//Calculate the combined force of the steering behavior
	SteeringForce = ComponentSteerBeh2D->Calculate();

	//Acceleratoin = Force/Mass
	FVector2DPlus Acceleration = SteeringForce / Mass;

	//Update velocity accounting for time since last frame
	Velocity += Acceleration * DeltaTime;

	//Make sure velocity does not exceed max speed
	Velocity.Truncate(MaxSpeed);

	if (GEngine)
	{
	//GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("SteeringForceis: %f, %f"), SteeringForce.X, SteeringForce.Y));
		//GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Green, FString::Printf(TEXT("Acceleration.Length() is: %f"), Acceleration.Size()));
		//GEngine->AddOnScreenDebugMessage(3, 15.0f, FColor::Green, FString::Printf(TEXT("Velocity is: %f"), Velocity.Size()));
	}

	//Update location accounting for time since last from last frame
	Location2D += Velocity * DeltaTime;

	//Sets Speed so we can see it in Unreal Editor, can be removed if used in game as it does nothing
	Speed = Velocity.Size();

	//Updates Location of actor
	Component2D->SetActorLocation2D(Location2D);

	//Update heading if the actor is moving
	if (Velocity.SizeSquared() > .000000001)
	{
		//Normalize vecolicity to get heading
		Heading = Velocity.GetNormal();

		//Grab rotation from actor
		FRotator ActorRotation = this->GetActorRotation();

		//Update angles.  Make sure you include Yaw and Roll otherwise they change a bit causing visual glitches.
		//In this game we are only rotating around the Y axis.
		ActorRotation.Pitch = FMath::RadiansToDegrees(FMath::Atan2(Heading.Y, Heading.X));
		ActorRotation.Yaw = 0.f;
		ActorRotation.Roll = 0.f;

		//Actually rotate actor to new angle
		SetActorRotation(ActorRotation);

		//Check to see if player needs wrapping
		if (GameMode)
		{
			GameMode->WrapAround(Location2D);
		}
	}

	if (bDrawDebugLines)
	{
		DrawDebugCircle(
			GetWorld(),
			GetActorLocation(),
			Component2D->GetRadius(),
			32,
			FColor(100, 200, 100),
			false,
			-1,
			0,
			3,
			FVector(1, 0, 0),
			FVector(0, 0, 1),
			false
		);
	}
}
