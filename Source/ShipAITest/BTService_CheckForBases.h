// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForBases.generated.h"

/**
 * 
 */
UCLASS()
class SHIPAITEST_API UBTService_CheckForBases : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_CheckForBases();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
