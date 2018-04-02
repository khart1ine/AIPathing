// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "MovementGameModeBase.h"
#include "DrawDebugHelpers.h"
#include "PaperSprite.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Sprite SubObject
	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Player Sprite"));
	RootComponent = PaperSpriteComponent;

	Component2D = CreateDefaultSubobject<UActorComponent2D>(TEXT("2D Actor Component"));

	//Set Default variable values
	MaxPlayerSpeed = 200.0f;
	Force = 150.0f;
	MaxTurnSpeed = 150.0f;
	Friction = 5.0f;
	Rotation = 0.0f;
	Pitch = 0.01f;
	Velocity = FVector2DPlus(0.0f, 0.0f);
	Location2D = FVector2DPlus(0.0f, 0.0f);

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	//Gets world locatoin in level for placed AActor
	Location2D.X = GetActorLocation().X;
	Location2D.Y = GetActorLocation().Z;
	
	//Accesses current GameModeBase
	AGameModeBase* GMB = UGameplayStatics::GetGameMode(this);

	if (GMB)
	{
		//Up casts to our special gamemode base that holds camera reference
		GameMode = (AMovementGameModeBase *)GMB;
	}

	//Get Actor Radius
	UPaperSprite* PaperSprite = PaperSpriteComponent->GetSprite();

	if (PaperSprite)
	{
		Component2D->SetRadius(FVector2DPlus::Diagonal(PaperSprite->GetSourceSize().X, PaperSprite->GetSourceSize().Y, Component2D->GetCollisionRadiusAdjustment()) / 2);
	}
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Player turns with left and right button in degrees.  Positive goes counter clockwise.
	Rotation = -XInputAxis * MaxTurnSpeed * DeltaTime;
	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Rotation is: %f"), Rotation));
	//Gets Rotator
	FRotator ActorRotation = this->GetActorRotation();

	//Clamp rotation between -360f and 360f with FModulus
	Pitch = FMath::Fmod(Pitch + Rotation, 360.0f);

	//Change pitch, but also need to lock Yaw and Roll otherwise they adjust fractionally which flips the axis
	ActorRotation.Pitch = Pitch; //Cant just add to Pitch (+=) as this gets clamped from 90 to -90
	ActorRotation.Yaw = 0.0f;
	ActorRotation.Roll = 0.0f;

	//Actually affects the new ActorRotation angle on AActor
	SetActorRotation(ActorRotation);
	
	


	//Get speed from controller
	float Speed = Force * YInputAxis * DeltaTime;

	//Add to velocity with current direction and speed
	Velocity += FVector2DPlus::SphericalToVector(Pitch, Speed);

	//Clamp Max Speed
	Velocity.Truncate(MaxPlayerSpeed);

	//Add Friction
	if (Velocity.Size() > Friction * DeltaTime) //Ensure friction is less that current velocity
	{
		Velocity.Truncate(Velocity.Size() - Friction * DeltaTime);
	}
	else //friction stops it completely
	{
		Velocity.X = 0.f;
		Velocity.Y = 0.f;
	}

	//Update position vector
	Location2D += Velocity * DeltaTime;

	Component2D->SetActorLocation2D(Location2D);

	if (GameMode)
	{
		GameMode->WrapAround(Location2D);
	}

	if (bDrawDebugLines)
	{
		DrawDebugCircle(
			GetWorld(),
			GetActorLocation(),
			Component2D->GetRadius(),
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
	}


}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//Calls MovementUp() and MovementRight() if Axis Movement is set properly in Project Settings
	PlayerInputComponent->BindAxis("Up", this, &APlayerPawn::MovementUp);
	PlayerInputComponent->BindAxis("Right", this, &APlayerPawn::MovementRight);



}

/** Update inputs for Horizontal axis**/
void APlayerPawn::MovementRight(float AxisValue)
{
	//set X axis
	XInputAxis = AxisValue;
}

/** Update inputs for vertical axis**/
void APlayerPawn::MovementUp(float AxisValue)
{
	//set Y axis
	YInputAxis = AxisValue;

}

