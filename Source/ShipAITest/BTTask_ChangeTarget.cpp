// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_ChangeTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "ShipAI.h"
#include "ThirdPersonTestCharacter.h"
#include "ShipAITestPawn.h"
#include "ShipCharacter.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"


EBTNodeResult::Type UBTTask_ChangeTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AShipAI *CharPC = Cast<AShipAI>(OwnerComp.GetAIOwner());

	AThirdPersonTestCharacter *Player = Cast<AThirdPersonTestCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->PlayerID));
	AShipAITestPawn *FlyingPlayer = Cast<AShipAITestPawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->FlyingPlayerID));

	FVector HeadingTowards;

	switch (CharPC->BaseHeading)
	{
	case 0:
		HeadingTowards = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		if ((HeadingTowards - CharPC->GetPawn()->GetActorLocation()).Size() < 10.f) (CharPC->BaseHeading)++;
		return EBTNodeResult::Succeeded;
	case 1:
		HeadingTowards = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base1ID))->GetActorLocation();
		if ((HeadingTowards - CharPC->GetPawn()->GetActorLocation()).Size() < 5.f) (CharPC->BaseHeading)++;
		return EBTNodeResult::Succeeded;
	case 2:
		HeadingTowards = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base2ID))->GetActorLocation();
		if ((HeadingTowards - CharPC->GetPawn()->GetActorLocation()).Size() < 5.f) (CharPC->BaseHeading)++;
		return EBTNodeResult::Succeeded;
	case 3:
		HeadingTowards = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base3ID))->GetActorLocation();
		if ((HeadingTowards - CharPC->GetPawn()->GetActorLocation()).Size() < 5.f) (CharPC->BaseHeading)++;
		return EBTNodeResult::Succeeded;
	case 4:
		HeadingTowards = Cast<ATargetPoint>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->Base4ID))->GetActorLocation();
		if ((HeadingTowards - CharPC->GetPawn()->GetActorLocation()).Size() < 5.f) CharPC->BaseHeading = 0;
		return EBTNodeResult::Succeeded;
	default:
		GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Red, "ERROR: Target Number not recognized");
		return EBTNodeResult::Failed;
	}

	EBTNodeResult::Failed;
}


