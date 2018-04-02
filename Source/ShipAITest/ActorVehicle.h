// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorComponentSteeringBehavior.h"
#include "PaperSpriteComponent.h"
#include "ActorComponent2D.h"
#include "MovementWalls.h"
#include "MovementPath.h"
#include "ActorVehicle.generated.h"

UCLASS()
class SHIPAITEST_API AActorVehicle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorVehicle();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "2D", meta = (AllowPrivateAccess = "true"))
	UActorComponent2D* Component2D;

	/** Returns the maximum magnitude of the player's allowed velocity **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	inline float GetMaxSpeed()const { return MaxSpeed; }

	/** Returns the maximum magnitude of the player's allowed velocity **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	inline float GetSpeed()const { return Speed; }

	/** Gets current 2D velocity vector **/
	UFUNCTION(BlueprintPure, Category = "Movement")
	inline FVector2DPlus Get2DVelocity()const { return Velocity; }

	/** Get's the static non moving target location in world space **/
	UFUNCTION(BlueprintPure, Category = "Target")
	FVector2DPlus GetSBTargetLocation()const;

	/** Get's the static non moving target location in world space **/
	UFUNCTION(BlueprintPure, Category = "Target")
	AActor* GetTargetPtr()const {return Target;}

	/** Returns the max force which is the most a player can create on their own **/
	UFUNCTION(BlueprintPure, Category = "Movement")
	float GetMaxForce()const { return MaxForce; }

	/** Returns the normalized heading in vehicle **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE FVector2DPlus GetHeading() const { return Heading; }

	/** Returns the normalized heading in vehicle **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE float GetAngleInDegrees() const { return FMath::RadiansToDegrees( Heading.GetAngle()); }

	/** Returns the max force which is the most a player can create on their own **/
	UFUNCTION(BlueprintPure, Category = "Debug")
	bool IsDrawDebugLines()const { return bDrawDebugLines; }

	/** Returning if pointer is AActor NPC or APawn Player **/
	UFUNCTION(BlueprintCallable, Category = "Steering Behvavior")
	bool IsActorNPC()const {return bIsNPC;}

	/** Returns the time since last frame in milliseconds **/
	UFUNCTION(BlueprintPure, Category = "2DLocation")
	FORCEINLINE float GetDeltaTime() const { return DeltaTimeForActor; }

	/** Getting reference to gamemode holding obstacle TArray **/
	UFUNCTION(BlueprintCallable, Category = "ObstaclesRef")
	void  PrintDebugLineFromPlayerOrigin(FVector2DPlus End, FColor Color, bool Local)const;

	/** Getting reference to gamemode holding obstacle TArray **/
	UFUNCTION(BlueprintCallable, Category = "ObstaclesRef")
	TArray<class AMovementObstacle *>&  GetGameModeObstacles()const;

	/** Getting reference to gamemode holding walls TArray **/
	UFUNCTION(BlueprintCallable, Category = "WallsRef")
	TArray<struct FWallType>&  GetGameModeWalls()const;

	/** Getting reference to gamemode holding walls TArray **/
	UFUNCTION(BlueprintCallable, Category = "WallsRef")
	TArray<AActorVehicle*>&  GetAgentsInLevel()const;

	/** Getting reference to gamemode for SteeringBehaviros**/
	UFUNCTION(BlueprintPure, Category = "Target")
	class AMovementGameModeBase* GetGameMode()const { return GameMode; }

	/** Getting reference to gamemode for SteeringBehaviros**/
	UFUNCTION(BlueprintPure, Category = "Interpose")
	class AActorVehicle* GetInterpose1()const { return Interpose1; }

	/** Getting reference to gamemode for SteeringBehaviros**/
	UFUNCTION(BlueprintPure, Category = "Interpose")
	class AActorVehicle* GetInterpose2()const { return Interpose2; }

	/** Getting reference to gamemode holding walls TArray **/
	UFUNCTION(BlueprintCallable, Category = "WallsRef")
	AMovementPath*  GetMovementPath()const { return MovementPath; }

	/** Gets current 2D velocity vector **/
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void VelocityZero() { Velocity.Zero(); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** contains sprite component for player **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PaperSprite", meta = (BlueprintProtect = "true"))
	UPaperSpriteComponent * PaperSpriteComponent;

	/** Vehicle Maximum Speed controls the magnitude of the Velocity**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float MaxSpeed;

	/** The maximum for this entity can produce to power itself **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float MaxForce;

	/** Static Target set in world **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target", meta = (BlueprintProtect = "true", AllowedClasses = "AActorVehicle, APlayerPawn"))
	AActor* Target;

	/** Mass of Actor **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	float Mass;

	/** Determines if Target is AActor NPC or ASPawn Player **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steering Behavior", meta = (BlueprintProtect = "true"))
	bool bIsNPC;

	/** Turn Debug Line Drawing from Actor to Steering Force **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (BlueprintProtect = "true"))
	bool bDrawDebugLines;

	/** Two targets to get to center of for interpose SB**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpose", meta = (BlueprintProtect = "true"))
	AActorVehicle* Interpose1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interpose", meta = (BlueprintProtect = "true"))
	AActorVehicle* Interpose2;

	/** Stores path for vehicle **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow Path Behavior", meta = (BlueprintProtect = "true"))
	class AMovementPath * MovementPath;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "2D", meta = (AllowPrivateAccess = "true"))
	UActorComponentSteeringBehavior* ComponentSteerBeh2D;

	//Vehicle velocity
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Velocity;

	/** 2D Location of Actor (X, Y) **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Location2D;

	/**a normalized vector pointing in the direction that the actor is heading  **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2DPlus Heading;

	/**a normalized vector pointing in the direction that the actor is heading  **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float Speed;

	/** Holds the game mode so player can reference camera **/
	UPROPERTY()
	class AMovementGameModeBase* GameMode;
	
	/** time in miliseconds from last frame **/
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float DeltaTimeForActor;

};
