// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
#include "Camera/CameraComponent.h"
#include "MovementObstacle.h"

// Sets default values for this actor's properties
AMovementGameModeBase::AMovementGameModeBase()
{
	Width = 1920.0f;
	Height = 0.0f;
}

//Returns the four extremes of the camera frustum to determine if actors are on orthographic screen
Frustum AMovementGameModeBase::GetCameraExtents()
{
	//Stores the four extents of the orthographic camera frustum
	Frustum ExtentCamFrustum;

	if (CameraComponent)
	{

		FMinimalViewInfo cameraView;

		//Gets active camera in scene
		CameraComponent->GetCameraView(0.0f, cameraView);

		//Since camera points at middle of scene we need to subtract half the height and width to get the extents
		ExtentCamFrustum.TopCameraFrustum = cameraView.Location.Z + Height / 2.0f;
		ExtentCamFrustum.BottomCameraFrustum = cameraView.Location.Z - Height / 2.0f;

		ExtentCamFrustum.LeftCameraFrustum = cameraView.Location.X - Width / 2.0f;
		ExtentCamFrustum.RightCameraFrustum = cameraView.Location.X + Width / 2.0f;
	}

	return ExtentCamFrustum;
}

// Called when the game starts or when spawned
void AMovementGameModeBase::BeginPlay()
{
	/** Something in unreal spawns a default CameraActor.  This Destroys it**/
	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ACameraActor *CameraActor = *ActorItr;
		CameraActor->Destroy(); //Will spawn proper camera below
	}
	Super::BeginPlay();

	//Set camera position and rotate it 90% on X Axis (Yaw) - Constructor changes the normal display order
	FVector Location(960.0f, 10.0f, 540.0f);
	FRotator Rotation(0.0f, -90.0f, 0.0f);

	//Create a new camera 
	MainCamera = GetWorld()->SpawnActor<ACameraActor>(Location, Rotation);

	if (MainCamera)
	{
		//Get new Camera Component
		CameraComponent = MainCamera->GetCameraComponent();
		if (CameraComponent)
		{
			//Set to Orthographic, set the width of the frame and calculate height based on aspect ration of camera
			CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
			CameraComponent->OrthoWidth = Width;
			Height = Width / CameraComponent->AspectRatio;
		}
	}

	//Get reference to PlayerController in scene
	APlayerController* PlayerController =  UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetViewTarget(MainCamera); //Set as main camera
	}

}

/** Screen wraps the actdor so they are always in view**/
void AMovementGameModeBase::WrapAround(FVector2DPlus & Pos, float Margin)
{
		Frustum NewCamExtents = GetCameraExtents();
		//Add margin delaying wrap if positive
		if (Margin)
		{
			NewCamExtents.LeftCameraFrustum -= Margin;
			NewCamExtents.TopCameraFrustum += Margin;
			NewCamExtents.RightCameraFrustum += Margin;
			NewCamExtents.BottomCameraFrustum -= Margin;
		}

		//Screen wraps from right side to left side
		if (Pos.X > NewCamExtents.RightCameraFrustum) { Pos.X = NewCamExtents.LeftCameraFrustum; }

		//Screen wraps from left side to right side
		if (Pos.X < NewCamExtents.LeftCameraFrustum) { Pos.X = NewCamExtents.RightCameraFrustum; }

		//Screen wraps from top to bottom
		if (Pos.Y > NewCamExtents.TopCameraFrustum) { Pos.Y = NewCamExtents.BottomCameraFrustum; }

		//Screen wraps from bottom to top
		if (Pos.Y < NewCamExtents.BottomCameraFrustum) { Pos.Y = NewCamExtents.TopCameraFrustum; }
}

void AMovementGameModeBase:: TagNeighbors(AMovementVehicle * VehiclePtr, const float radius)
{

	//iterate through all entities checking for range
	//for (it = others.begin(); it != others.end(); ++it)
	for (auto& Ob : ObstaclesPtr)
	{
		//first clear any current tag
		Ob->TagFalse();

		//work in distance squared to avoid sqrts
		FVector2DPlus To = Ob->GetActorLocation2D() - VehiclePtr->GetActorLocation2D();

		//the bounding radius of the other is taken into account by adding it 
		//to the range
		float Range = radius + Ob->GetRadius();

		//if entity within range, tag for further consideration
		if (To.SizeSquared() < Range*Range)
		{
			Ob->TagTrue();
		}

	}//next entity
}