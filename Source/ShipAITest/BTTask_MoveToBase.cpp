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


EBTNodeResult::Type UBTTask_MoveToBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AShipAI *CharPC = Cast<AShipAI>(OwnerComp.GetAIOwner());

	AThirdPersonTestCharacter *Base = Cast<AThirdPersonTestCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base1ID));

	if (Base)
	{
		CharPC->MoveToActor(Base, 5.f, true, true, true, 0, true);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}

	EBTNodeResult::Failed;
}