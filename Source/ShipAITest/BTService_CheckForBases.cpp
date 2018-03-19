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
		AThirdPersonTestCharacter *Player = Cast<AThirdPersonTestCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		AShipAITestPawn *FlyingPlayer = Cast<AShipAITestPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (Player)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(ShipPC->PlayerID, Player);
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, "Found ground unit");
			
		}
		else if (FlyingPlayer)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(ShipPC->FlyingPlayerID, FlyingPlayer);
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, "Found UFO");
		}
	}
}
