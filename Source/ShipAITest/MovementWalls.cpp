// Fill out your copyright notice in the Description page of Project Settings.

#include "MovementWalls.h"
#include "PaperSprite.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"


AMovementWalls::AMovementWalls()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Sprite SubObject
	PaperSpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Player Sprite"));
	RootComponent = PaperSpriteComponent;
}

void AMovementWalls::BeginPlay()
{

	Super::BeginPlay();

	//Game Game mode base so we can add instance to array
	AGameModeBase* GMB = UGameplayStatics::GetGameMode(this);

	if (GMB)
	{
		GameMode = (AMovementGameModeBase *)GMB;
	}

	///Get Sprite component from BluePrint
	UPaperSprite* PaperSprite = PaperSpriteComponent->GetSprite();

	if (PaperSprite) //Make sure there is a sprite component and it is set
	{
		/** Get the extents in PaperSprite, assumption that has not been altered  with pivot 
			This is the radius from the center of the rectangle										**/
		FVector2DPlus LeftExtent;
		FVector2DPlus RightExtent;
		FVector2DPlus Extent;
		FBox CBB = PaperSprite->GetRenderBounds().GetBox();

		/** Account for scale, need to check changing pixel render size next **/
		Extent.X = CBB.GetExtent().X * GetActorScale().X;
		Extent.Y = CBB.GetExtent().Z * GetActorScale().Z;

		/** Project points to extents of edges in center**/
		LeftExtent = FVector2DPlus(GetActorForwardVector().X * -Extent.X, GetActorForwardVector().Z * -Extent.X);
		RightExtent = FVector2DPlus(GetActorForwardVector().X * Extent.X, GetActorForwardVector().Z * Extent.X);
		/** Project from edge to top corners anc convert to world coordinates**/
		TopLeft = FVector2DPlus(GetActorUpVector().X * Extent.Y, GetActorUpVector().Z * Extent.Y) + LeftExtent + GetActorLocation2D();
		TopRight = FVector2DPlus(GetActorUpVector().X * Extent.Y, GetActorUpVector().Z * Extent.Y) + RightExtent + GetActorLocation2D();
		
		/** Project from edge to bottom corners and convert to world coordinates**/
		BottomLeft = FVector2DPlus(GetActorUpVector().X * -Extent.Y, GetActorUpVector().Z * -Extent.Y) + LeftExtent + GetActorLocation2D();
		BottomRight = FVector2DPlus(GetActorUpVector().X * -Extent.Y, GetActorUpVector().Z * -Extent.Y) + RightExtent + GetActorLocation2D();

	}

	if (GameMode)
	{
		//FWallType Line1 = FWallType(
		//GameMode->WallsPtr.AddUnique(TopLeft, TopRight, this);
	}

}

void AMovementWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDrawDebugLines)
	{
		PrintDebug(TopLeft, TopRight);
		PrintDebug(TopLeft, BottomLeft);
		PrintDebug(BottomLeft, BottomRight);
		PrintDebug(TopRight, BottomRight);
	}
}

void AMovementWalls::PrintDebug(FVector2DPlus Start, FVector2DPlus End)
{
	DrawDebugLine(
		GetWorld(),
		FVector(Start.X, 0, Start.Y),
		FVector(End.X, 0, End.Y),
		FColor(255, 0, 0),
		true,  //Persistent
		-1,
		0,
		2	//thickness
	);
}


