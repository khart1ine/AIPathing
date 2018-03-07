// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ShipCharacter.h"
#include "ShipAITest.h"

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

		Base1ID = BlackboardComp->GetKeyID("Base1");
		Base2ID = BlackboardComp->GetKeyID("Base2");

		BehaviorComp->StartTree(*Ship->ShipBehavior);
	}
}

