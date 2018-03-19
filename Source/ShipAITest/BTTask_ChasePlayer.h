// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ShipAI.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class SHIPAITEST_API UBTTask_ChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	float AbsoluteDistance(AActor* Target, UBehaviorTreeComponent& OwnerComp)
	{
		AShipAI *CharPC = Cast<AShipAI>(OwnerComp.GetAIOwner());
		return (Target->GetActorLocation() - CharPC->GetPawn()->GetActorLocation()).Size();
	}
	
};
