// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShipAI.generated.h"

/**
 * 
 */
UCLASS()
class SHIPAITEST_API AShipAI : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(transient)
	class UBlackboardComponent *BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent *BehaviorComp;

public:

	AShipAI();

	virtual void Possess(APawn *InPawn) override;
	
	uint8 Base1ID;
	uint8 Base2ID;
};
