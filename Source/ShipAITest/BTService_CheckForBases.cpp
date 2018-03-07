// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForBases.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ShipAITestPawn.h"
#include "ThirdPersonTestCharacter.h"
#include "ShipCharacter.h"
#include "ShipAITest.h"
#include "ShipAI.h"


UBTService_CheckForBases::UBTService_CheckForBases()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForBases::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AShipAI *ShipPC = Cast<AShipAI>(OwnerComp.GetAIOwner());

	if (ShipPC)
	{
		AThirdPersonTestCharacter *Base = Cast<AThirdPersonTestCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (Base)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(ShipPC->Base1ID, Base);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "Enemy is here!");
		}
	}
}
