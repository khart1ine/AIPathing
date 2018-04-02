// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementObstacle.h"
#include "DrawDebugHelpers.h"
#include "PaperSprite.h"
#include "Kismet/GameplayStatics.h"
#include "MovementGameModeBase.h"

AMovementObstacle::AMovementObstacle()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Sprite SubObject
	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Player Sprite"));
	RootComponent = PaperSpriteComponent;

	Component2D = CreateDefaultSubobject<UActorComponent2D>(TEXT("2D Actor Component"));
}

void AMovementObstacle::BeginPlay()
{

	Super::BeginPlay();

	//Game Game mode base so we can add instance to array
	AGameModeBase* GMB = UGameplayStatics::GetGameMode(this);

	if (GMB)
	{
		GameMode = (AMovementGameModeBase *)GMB;
	}


	UPaperSprite* PaperSprite = PaperSpriteComponent->GetSprite();

	if (PaperSprite)
	{
		Component2D->SetRadius (FVector2DPlus::Diagonal(PaperSprite->GetSourceSize().X * GetActorScale().X, PaperSprite->GetSourceSize().Y * GetActorScale().Z, Component2D->GetCollisionRadiusAdjustment()) / 2);
	}

	if (GameMode)
	{
		GameMode->ObstaclesPtr.AddUnique(this);
	}
}


void AMovementObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

