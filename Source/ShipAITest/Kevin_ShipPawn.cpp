// Fill out your copyright notice in the Description page of Project Settings.

#include "Kevin_ShipPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "EngineGlobals.h"


// Sets default values
AKevin_ShipPawn::AKevin_ShipPawn()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		//ConstructorHelpers::FObjectFinderOptional<UParticleSystem> ParticleTemplate;
		//ConstructorHelpers::FObjectFinderOptional<UParticleSystem> SparkParticleTemplate;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Geometry/Meshes/SM_cockpit_test_final"))//,
			//ParticleTemplate(TEXT("/Game/Particles/PS_ShipLaser")),
			//SparkParticleTemplate(TEXT("/Game/Particles/PS_Sparks"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates the Static Mesh component of the ship and sets it as the root
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	MeshComponent->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = MeshComponent;

	// Adds a scene component to be used for moving the camera position and gives a relative offset to the mesh
	VRCameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraRoot"));
	VRCameraRoot->SetupAttachment(RootComponent);
	VRCameraRoot->RelativeLocation = FVector(71.0f, 0.0f, 137.0f);

	// Creates the camera to be used for the player
	VRCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCameraComponent->SetupAttachment(VRCameraRoot);

	/*
	// Sets up Laser system
	LaserRoot = CreateDefaultSubobject<USceneComponent>(FName("LaserRoot"));
	LaserRoot->SetupAttachment(RootComponent);
	LaserRoot->RelativeLocation = FVector(30.f, -263.f, 66.f);

	// This is a marker that is used as the laser's source point
	LaserStartPoint = CreateDefaultSubobject<UStaticMeshComponent>(FName("LaserStartPoint"));
	LaserStartPoint->SetupAttachment(LaserRoot);
	LaserStartPoint->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	// This is a marker that is used as the laser's endpoint when there is no collision
	LaserEndPoint = CreateDefaultSubobject<UStaticMeshComponent>(FName("LaserEndPoint"));
	LaserEndPoint->SetupAttachment(LaserRoot);
	LaserEndPoint->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	// The actal particle system of the laser 
	LaserParticles = CreateDefaultSubobject<UParticleSystemComponent>(FName("LaserParticles"));
	LaserParticles->SetupAttachment(LaserRoot);
	//LaserParticles->SetTemplate(ConstructorStatics.ParticleTemplate.Get());
	LaserParticles->SetBeamSourcePoint(0, LaserStartPoint->GetComponentLocation(), 0);
	LaserParticles->SetBeamTargetPoint(0, LaserEndPoint->GetComponentLocation(), 0);

	// A sub-particle system that activates during a collision with the laser
	SparkParticles = CreateDefaultSubobject<UParticleSystemComponent>(FName("SparkParticles"));
	SparkParticles->SetupAttachment(LaserEndPoint);
	//SparkParticles->SetTemplate(ConstructorStatics.SparkParticleTemplate.Get());
	SparkParticles->SetRelativeRotation(FRotator(0.f, 140.f, 0.f));
	SparkParticles->DeactivateSystem();

	// Sets up drill system
	DrillRoot = CreateDefaultSubobject<USceneComponent>(FName("DrillRoot"));
	DrillRoot->SetupAttachment(RootComponent);
	DrillRoot->RelativeLocation = FVector(180.f, 253.f, 66.f);

	// This is the mesg that will represent the drill
	DrillMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("DrillMesh"));
	DrillMesh->SetupAttachment(DrillRoot);
	DrillMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	*/

	// The component that handles ship flight controls
	MovementComponent = CreateDefaultSubobject<UKevin_ShipMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Initializing some default values
	MaxLaserDistance = 4000.f;
	MaxDrillDistance = 500.f;
	bLaserPressed = false;
	bDrillPressed = false;

	RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
}

UKevin_ShipMovementComponent* AKevin_ShipPawn::GetMovementComponent() const
{
	return MovementComponent;
}

// Called when the game starts or when spawned
void AKevin_ShipPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKevin_ShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//LaserParticles->SetBeamSourcePoint(0, LaserStartPoint->GetComponentLocation(), 0);
	//LaserParticles->SetBeamTargetPoint(0, LaserEndPoint->GetComponentLocation(), 0);

	//ShootLaser(bLaserPressed);
	//ShootDrill(bDrillPressed);

}

// Called to bind functionality to input
void AKevin_ShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Kevin_LeftStickY", MovementComponent, &UKevin_ShipMovementComponent::MoveForward);
	PlayerInputComponent->BindAxis("Kevin_LeftStickX", MovementComponent, &UKevin_ShipMovementComponent::SlideRight);
	PlayerInputComponent->BindAxis("Kevin_FaceUpDown", MovementComponent, &UKevin_ShipMovementComponent::SlideUp);
	PlayerInputComponent->BindAxis("Kevin_RightStickX", MovementComponent, &UKevin_ShipMovementComponent::Yaw);
	PlayerInputComponent->BindAxis("Kevin_RightStickY", MovementComponent, &UKevin_ShipMovementComponent::Pitch);
	PlayerInputComponent->BindAxis("Kevin_Shoulders", MovementComponent, &UKevin_ShipMovementComponent::Roll);

}

void AKevin_ShipPawn::OnResetVR()
{
	//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AKevin_ShipPawn::LaserPressed()
{
	bLaserPressed = true;
}

void AKevin_ShipPawn::LaserReleased()
{
	bLaserPressed = false;
}

void AKevin_ShipPawn::DrillPressed()
{
	bDrillPressed = true;
}

void AKevin_ShipPawn::DrillReleased()
{
	bDrillPressed = false;
}

void AKevin_ShipPawn::ShootLaser(bool bPressed)
{
	/*float LaserLength = LaserEndPoint->RelativeLocation.X;

	if (bPressed)
	{
		//Re-initialize hit info
		FHitResult RV_Hit(ForceInit);

		//call GetWorld() from within an actor extending class
		GetWorld()->LineTraceSingleByChannel(
			RV_Hit,											//result
			LaserStartPoint->GetComponentLocation(),		//start
			LaserEndPoint->GetComponentLocation(),			//end
			ECC_Pawn,										//collision channel
			RV_TraceParams
		);

		// If the laser intersected with anything, it sets the endpoint of the location to the site of impact
		// It also activates the particles at the source of impact
		if (RV_Hit.bBlockingHit)
		{
			float ImpactDistance = (RV_Hit.ImpactPoint - LaserStartPoint->GetComponentLocation()).Size();
			LaserEndPoint->SetRelativeLocation(FVector(ImpactDistance, 0.f, 0.f));
			SparkParticles->ActivateSystem();
		}
		else
		{
			SparkParticles->DeactivateSystem();
			if (LaserLength >= MaxLaserDistance)
			{
				LaserEndPoint->SetRelativeLocation(FVector(MaxLaserDistance, 0.f, 0.f));
			}
			else if (LaserLength < MaxLaserDistance)
			{
				LaserEndPoint->SetRelativeLocation(FVector(LaserLength + 200.f, 0.f, 0.f));
			}
		}
	}
	else
	{
		SparkParticles->DeactivateSystem();
		if (LaserLength <= 0.f)
		{
			LaserEndPoint->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		}
		else if (LaserLength > 0.f)
		{
			LaserEndPoint->SetRelativeLocation(FVector(LaserLength - 100.f, 0.f, 0.f));
		}
	}*/
}

void AKevin_ShipPawn::ShootDrill(bool bPressed)
{
	/*float DrillDistance = DrillMesh->RelativeLocation.X;

	if (bPressed)
	{
		DrillMesh->AddRelativeRotation(FRotator(0.f, 0.f, 10.f));
		if (DrillDistance >= MaxDrillDistance)
		{
			DrillMesh->SetRelativeLocation(FVector(MaxDrillDistance, 0.f, 0.f));
		}
		else if (DrillDistance < MaxDrillDistance)
		{
			DrillMesh->SetRelativeLocation(FVector(DrillDistance + 10.f, 0.f, 0.f));
		}
	}
	else
	{
		if (DrillDistance <= 0.f)
		{
			DrillMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		}
		else if (DrillDistance > 0.f)
		{
			DrillMesh->SetRelativeLocation(FVector(DrillDistance - 5.f, 0.f, 0.f));
		}
	}*/
}

