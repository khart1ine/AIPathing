// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementVehicle.h"
#include "MovementGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "PaperSprite.h"
#include "EngineGlobals.h"



AMovementVehicle::AMovementVehicle()
{

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializes default variables
	MaxSpeed = 180.0f;


	MaxForce = 120.0f;

	//MaxTurnRate = .1f;

	Mass = 1.f;

	bDrawDebugLines = false;

	/** Wander & Evade Behavior **/
	WanderRadius = 100.0f;
	WanderJitter = 1000.0f;
	WanderDistance = 200.0f;
	LookAheadPursuit = 2.0f;

	/** Arrive Behavior **/
	DecelerationTweaker = 3;


	CollisionRadiusAdjustment = -32.0f;

	//Setup Sprite SubObject
	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Player Sprite"));
	RootComponent = PaperSpriteComponent;

	/** Default settings for Behavior Types weights**/
	WeightSeek = 1.0f;
	WeightFlee = 1.0f;
	WeightArrive = 1.0f;
	WeightPursuit = 1.0f;
	WeightEvade = 1.0f;
	WeightWander = 1.0f;
	WeightObstacleAvoidance = 1.0f;
	WeightWallAvoidance = 1.0F;
}

TArray<class AMovementObstacle*>& AMovementVehicle::GetGameModeObstacles() const
{
	return GameMode->ObstaclesPtr;
}

TArray<struct FWallType>& AMovementVehicle::GetGameModeWalls() const
{
	return GameMode->WallsPtr;
}


void AMovementVehicle::PrintDebugLineFromPlayerOrigin(FVector2DPlus End, FColor Color, bool Local) const
{
		//FVector Start = (GetActorForwardVector() * Radius) + GetActorLocation();
	FVector Start = GetActorLocation();
	FVector Finish;
	
	(Local) ? Finish=FVector(Start.X + End.X, 0.0F, Start.Z + End.Y) : Finish=FVector(End.X, 0.0f, End.Y);

		End *= DeltaTimeForActor * 10.0f ;
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
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(13, 150.0f, FColor::Yellow,
				FString::Printf(TEXT("Start: %f, %f - Finish: %f, %f"),
					Start.X, Start.Z, Finish.X, Finish.Z), false);

		}
}

void AMovementVehicle::BeginPlay()
{

	Super::BeginPlay();


	Steering = nullptr;

	/** Create an instance of the Steering Behavior class **/
	Steering = NewObject<USteeringBehaviors>();
	if (Steering != nullptr)
	{
		//Send SteeringBehavior pointer to this vehicle
		Steering->SetAgent(this);
		//Sends the default behavior for this vehicle
		Steering->SetWanderDistance(WanderDistance);
		Steering->SetWanderJitter(WanderJitter);
		Steering->SetWanderRadius(WanderRadius);
		Steering->SetLookAheadPursuit(LookAheadPursuit);
		Steering->SetDecelerationTweeker(DecelerationTweaker);

		/** Turn on and off behavior flags that are set in editor**/
		Steering->SetBehaviorFlags(BehaviorFlags);

		/** Set Weights **/
		Steering->SetBehaviorWeights(BehaviorTypes::Seek, WeightSeek);
		Steering->SetBehaviorWeights(BehaviorTypes::Flee, WeightFlee);
		Steering->SetBehaviorWeights(BehaviorTypes::Arrive, WeightArrive);
		Steering->SetBehaviorWeights(BehaviorTypes::Pursuit, WeightPursuit);
		Steering->SetBehaviorWeights(BehaviorTypes::Evade, WeightEvade);
		Steering->SetBehaviorWeights(BehaviorTypes::Wander, WeightWander);
		Steering->SetBehaviorWeights(BehaviorTypes::ObstacleAvoidance, WeightObstacleAvoidance);
		Steering->SetBehaviorWeights(BehaviorTypes::WallAvoidance, WeightWallAvoidance);
		Steering->SetBehaviorWeights(BehaviorTypes::Separation, WeightSeparation);
		Steering->SetBehaviorWeights(BehaviorTypes::Separation, WeightInterpose);
		Steering->SetBehaviorWeights(BehaviorTypes::Hide, WeightHide);
	}

	//Puts current location in editor into the Location for the 2D Struct
	Location2D = FVector2DPlus(0.0f, 0.0f);
	Location2D.X = GetActorLocation().X;
	Location2D.Y = GetActorLocation().Z;

	//Zero out the actor's velocity
	Velocity.Zero();

	//Game Game mode base so we can access camera
	AGameModeBase* GMB = UGameplayStatics::GetGameMode(this);

	if (GMB)
	{
		GameMode = (AMovementGameModeBase *)GMB;
		GameMode->VehiclesInLevelPtr.AddUnique(this);
	}

	//Set default heading to pointing due east
	Heading = FVector2DPlus(0.0f, 1.0f);

	UPaperSprite* PaperSprite = PaperSpriteComponent->GetSprite();

	if (PaperSprite)
	{
		Radius = FVector2DPlus::Diagonal(PaperSprite->GetSourceSize().X, PaperSprite->GetSourceSize().Y, CollisionRadiusAdjustment) / 2;
	}
}

void AMovementVehicle::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//Delta Time requied by outside function
	DeltaTimeForActor = DeltaTime;
	
	//Stores the upcoming force required by the vehicle
	FVector2DPlus SteeringForce;

	//Calculate the combined force of the steering behavior
	SteeringForce = Steering->Calculate();

	//Acceleratoin = Force/Mass
	FVector2DPlus Acceleration = SteeringForce / Mass;

	//Update velocity accounting for time since last frame
	Velocity += Acceleration * DeltaTime;

	//Make sure velocity does not exceed max speed
	Velocity.Truncate(MaxSpeed);

	//if (GEngine)
	//{
	//GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Green, FString::Printf(TEXT("SteeringForce.Length() is: %f, %f"), SteeringForce.X, SteeringForce.Y));
	//	GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Green, FString::Printf(TEXT("Acceleration.Length() is: %f"), Acceleration.Length()));
	//	GEngine->AddOnScreenDebugMessage(3, 15.0f, FColor::Green, FString::Printf(TEXT("Velocity is: %f"), Vel.Length()));
	//}

	//Update location accounting fro time since last from last frame
	Location2D += Velocity * DeltaTime;

	//Sets Speed so we can see it in Unreal Editor, can be removed if used in game as it does nothing
	Speed = Velocity.Size();

	//Updates Location of actor
	SetActorLocation2D(Location2D);

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
			Radius,
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

		if (Steering->IsWanderOn())
		{
			DrawDebugCircle(
				GetWorld(),
				(GetActorForwardVector()*WanderDistance) + GetActorLocation(),
				WanderRadius,
				32,
				FColor(255, 0, 0),
				false,
				-1,
				0,
				3,
				FVector(1, 0, 0),
				FVector(0, 0, 1),
				false
			);

			float TargetX = Steering->GetTarget().X;
			float TargetY = Steering->GetTarget().Y;
			DrawDebugLine(
				GetWorld(),
				(GetActorForwardVector()*WanderDistance) + GetActorLocation(),
				FVector(TargetX, 0.0f, TargetY),
				FColor(100, 100, 0),
				false,
				-1,
				0,
				3
			);
		}
	}
}

//Returns the world position of the static target
FVector2DPlus AMovementVehicle::GetTarget() const
{
	if (StaticTarget != nullptr)
	{
		return FVector2DPlus((double)StaticTarget->GetActorLocation().X, (double)StaticTarget->GetActorLocation().Z);
	}
	else
	{
		return FVector2DPlus(0.0, 0.0);
	}
}


