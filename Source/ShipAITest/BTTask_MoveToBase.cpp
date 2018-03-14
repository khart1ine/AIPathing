// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ShipAI.h"
#include "ThirdPersonTestCharacter.h"
#include "ShipAITestPawn.h"
#include "ShipCharacter.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"


EBTNodeResult::Type UBTTask_MoveToBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AShipAI *CharPC = Cast<AShipAI>(OwnerComp.GetAIOwner());

	AThirdPersonTestCharacter *Player = Cast<AThirdPersonTestCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->PlayerID));
	AShipAITestPawn *FlyingPlayer = Cast<AShipAITestPawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->FlyingPlayerID));

	AActor* Target = nullptr;
	FString TargetString = "";

	switch (CharPC->BaseHeading)
	{
	case 0:
		if (Player)
		{
			Target = Player;
			TargetString = "Player";
			break;
		}
		else if (FlyingPlayer)
		{
			Target = FlyingPlayer;
			TargetString = "FlyingPlayer";
			break;
		}
		else
		{
			break;
		}
	case 1:
		Target = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base1ID));
		TargetString = "Base1";
		break;
	case 2:
		Target = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base2ID));
		TargetString = "Base2";
		break;
	case 3:
		Target = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base3ID));
		TargetString = "Base3";
		break;
	case 4:
		Target = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base4ID));
		TargetString = "Base4";
		break;
	default:
		break;
	}

	if (Target)
	{
		CharPC->MoveToActor(Target, 5.f, true, true, true, 0, true);
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Yellow, "Moving towards Target: " + TargetString);
		if ((Target->GetActorLocation() - CharPC->GetPawn()->GetActorLocation()).Size() < 100.f)
		{
			CharPC->BaseHeading = (CharPC->BaseHeading + 1) % 5;

			GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Yellow, "Changing Target");
		}
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}
}