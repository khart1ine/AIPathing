// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ShipCharacter.h"
#include "ShipAITest.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"

AShipAI::AShipAI()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AShipAI::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);

	AShipCharacter *Ship = Cast<AShipCharacter>(InPawn);

	if (Ship && Ship->ShipBehavior)
	{
		BlackboardComp->InitializeBlackboard(*Ship->ShipBehavior->BlackboardAsset);

		PlayerID = BlackboardComp->GetKeyID("Player");
		FlyingPlayerID = BlackboardComp->GetKeyID("FlyingPlayer");

		Base1ID = BlackboardComp->GetKeyID("Base1");
		Base2ID = BlackboardComp->GetKeyID("Base2");
		Base3ID = BlackboardComp->GetKeyID("Base3");
		Base4ID = BlackboardComp->GetKeyID("Base4");
		LastKnownBaseID = BlackboardComp->GetKeyID("LastKnownBase");
		bFollowingPlayer = BlackboardComp->GetKeyID("FollowingPlayer");

		BaseHeading = 0;

		TActorIterator<ATargetPoint> ActorItr(GetWorld());
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(Base1ID, *ActorItr);
		++ActorItr;
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(Base2ID, *ActorItr);
		++ActorItr;
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(Base3ID, *ActorItr);
		++ActorItr;
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(Base4ID, *ActorItr);
		BlackboardComp->SetValue<UBlackboardKeyType_Bool>(bFollowingPlayer, false);

		BehaviorComp->StartTree(*Ship->ShipBehavior);
		RunBehaviorTree(Ship->ShipBehavior);
	}
}

void AShipAI::BeginPlay()
{
	Super::BeginPlay();
}

