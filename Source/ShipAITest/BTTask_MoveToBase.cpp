// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ShipAI.h"
#include "ThirdPersonTestCharacter.h"
#include "ShipCharacter.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "EngineGlobals.h"


EBTNodeResult::Type UBTTask_MoveToBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Reference to the Actor utilizing the AI
	AShipAI *CharPC = Cast<AShipAI>(OwnerComp.GetAIOwner());

	// Reference to Player Character
	AThirdPersonTestCharacter *Player = Cast<AThirdPersonTestCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->PlayerID));

	AActor* Target = nullptr;
	FString TargetString = "";

	// Switch statement to determine which checkpoint the AI is currently heading towards
	switch (CharPC->BaseHeading)
	{
	/*case 0:
		if (Player)
		{
			Target = Player;
			TargetString = "Player";
			break;
		}
		else
		{
			break;
		}*/
	case 0:
		Target = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base1ID));
		TargetString = "Base1";
		break;
	case 1:
		Target = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base2ID));
		TargetString = "Base2";
		break;
	case 2:
		Target = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base3ID));
		TargetString = "Base3";
		break;
	case 3:
		Target = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base4ID));
		TargetString = "Base4";
		break;
	default:
		break;
	}

	if (Target)
	{
		// Checks if the player exists first then whether the AI is close enough to the Player
		// if so, then the task fails to switch to the chase player task
		// Also, records the last base this AI was to head towards		
		if (Player)
		{
			GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Blue, FString::Printf(TEXT("Distance from last known base: %f"), AbsoluteDistance(Target, OwnerComp)));
			GEngine->AddOnScreenDebugMessage(4, 2.f, FColor::Orange, FString::Printf(TEXT("Distance from player: %f"), AbsoluteDistance(Player, OwnerComp)));
			if (AbsoluteDistance(Player, OwnerComp) <= 500.f &&
				AbsoluteDistance(Target, OwnerComp) <= 2000.f)
			{
				GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Purple, "Changing target to Player");
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(CharPC->LastKnownBaseID, Target);
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(CharPC->bFollowingPlayer, true);
				return EBTNodeResult::Failed;
			}
		}

		CharPC->MoveToActor(Target, 5.f, true, true, true, 0, true);
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Yellow, "Moving towards Target: " + TargetString);
		if (AbsoluteDistance(Target, OwnerComp) < 100.f)
		{
			CharPC->BaseHeading = (CharPC->BaseHeading + 1) % 4;

			//GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Yellow, "Changing Target");
		}
		return EBTNodeResult::Succeeded;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, "ERROR: Target not found");
		return EBTNodeResult::Aborted;
	}
}