// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementGameModeBase3D.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "EngineUtils.h"
//#include "Camera/CameraComponent.h"


AMovementGameModeBase3D::AMovementGameModeBase3D()
{
	Location = FVector(-1100.0f, 0.0f, 220.0f);
	Rotation = FRotator(0.0f, 0.0f, 0.0f);
}
void AMovementGameModeBase3D::BeginPlay()
{

	Super::BeginPlay();

	/** Something in unreal spawns a default CameraActor.  This Destroys it**/
	for (TActorIterator<ACameraActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ACameraActor *CameraActor = *ActorItr;
		CameraActor->Destroy(); //Will spawn proper camera below
	}

	//Create a new camera 
	MainCamera = GetWorld()->SpawnActor<ACameraActor>(Location, Rotation);

		//Get reference to PlayerController in scene
	APlayerController* PlayerController =  UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetViewTarget(MainCamera); //Set as main camera
	}

}

