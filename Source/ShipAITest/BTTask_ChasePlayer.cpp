// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ShipAI.h"
#include "ThirdPersonTestCharacter.h"
#include "ShipCharacter.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"


EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Reference to the Actor utilizing the AI
	AShipAI *CharPC = Cast<AShipAI>(OwnerComp.GetAIOwner());

	// Reference to Player Character
	AThirdPersonTestCharacter *Player = Cast<AThirdPersonTestCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->PlayerID));

	AActor* LastTarget = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->LastKnownBaseID));

	if (Player)
	{
		CharPC->MoveToActor(Player, 30.f, true, true, true, 0, true);
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Yellow, "Moving towards Player");
		GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Blue, FString::Printf(TEXT("Distance from last known base: %f"), AbsoluteDistance(LastTarget, OwnerComp)));
		GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Orange, FString::Printf(TEXT("Distance from player: %f"), AbsoluteDistance(Player, OwnerComp)));

		// If the player gets too far from the AI, or if it gets too far from the last checkpoint, it returns to checkpointing
		if (AbsoluteDistance(Player, OwnerComp) > 500.f ||
			AbsoluteDistance(LastTarget, OwnerComp) > 2000.f)
		{
			GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Purple, "Returning to base monitoring");
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(CharPC->bFollowingPlayer, false);
			return EBTNodeResult::Failed;
		}
		return EBTNodeResult::Succeeded;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, "ERROR: Player not found");
		return EBTNodeResult::Aborted;
	}
}



