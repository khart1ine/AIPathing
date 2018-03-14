// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "MovementGameModeBase.h"


// Sets default values
AMovementPlayer::AMovementPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Set Default variable values
	MaxPlayerSpeed = 50.0f;
	Force = 20.0f;
	MaxTurnSpeed = 150.0f;
	Friction = 5.0f;
	Rotation = 0.0f;
	Pitch = 0.01f;
	Velocity = FVector2DPlus (0.0f, 0.0f);
	Position = FVector2DPlus(0.0f, 0.0f);

}

// Called when the game starts or when spawned
void AMovementPlayer::BeginPlay()
{
	Super::BeginPlay();

	//Gets world locatoin in level for placed AActor
	Position.X = GetActorLocation().X;
	Position.Y = GetActorLocation().Z;

	//Accesses current GameModeBase
	AGameModeBase* GMB = UGameplayStatics::GetGameMode(this);

	if (GMB)
	{
		//Up casts to our special gamemode base that holds camera reference
		GameMode = (AMovementGameModeBase * )GMB;
	}
	
}

// Called every frame
void AMovementPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Player turns with left and right button in degrees.  Positive goes counter clockwise.
	Rotation = -XInputAxis * MaxTurnSpeed * DeltaTime;

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
	Position += Velocity * DeltaTime;

	SetActorLocation2D(Position);

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(10, 100.0f, FColor::Green, FString::Printf(TEXT("Speed: %f"), Speed));
		GEngine->AddOnScreenDebugMessage(11, 100.0f, FColor::Green, FString::Printf(TEXT("Velocity: %f"), Velocity.Size()));
		GEngine->AddOnScreenDebugMessage(12, 100.0f, FColor::Green, FString::Printf(TEXT("Rotation: %f"), Rotation));
	}*/

	//if (GameMode)
	//{
	//	Frustum Extents = GameMode->GetCameraExtents();

	//}
	
	if (GameMode)
	{
		GameMode->WrapAround(Position);
	}
	
}

// Called to bind functionality to input
void AMovementPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Calls MovementUp() and MovementRight() if Axis Movement is set properly in Project Settings
	PlayerInputComponent->BindAxis("Up", this, &AMovementPlayer::MovementUp);
	PlayerInputComponent->BindAxis("RIght", this, &AMovementPlayer::MovementRight);


}

/** Update inputs for Horizontal axis**/
void AMovementPlayer::MovementRight(float AxisValue)
{
	//set X axis
	XInputAxis = AxisValue;
}

/** Update inputs for vertical axis**/
void AMovementPlayer::MovementUp(float AxisValue)
{
	//set Y axis
	YInputAxis = AxisValue;

}

/** Sets the X and Y Value as well as the actual 3D location of AActor **/
bool AMovementPlayer::SetActorLocation2D(const FVector2DPlus & NewLocation)
{
	//Set the X, Y 2D FVector2DPlus Location
	Transform2D = NewLocation;

	//Move Player in World in its 3D Fvector level space
	return SetActorLocation(FVector(Transform2D.X, 0.0f, Transform2D.Y), false);

}

