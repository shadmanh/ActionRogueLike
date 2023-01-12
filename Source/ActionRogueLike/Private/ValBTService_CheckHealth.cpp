// Fill out your copyright notice in the Description page of Project Settings.


#include "ValBTService_CheckHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ValAICharacter.h"
#include "ValAttributeComponent.h"


void UValBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check health of bot

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AValAICharacter* SelfActor = Cast<AValAICharacter>(BlackBoardComp->GetValueAsObject("SelfActor"));
		if (SelfActor)
		{
			BlackBoardComp->SetValueAsBool(IsLowHealthKey.SelectedKeyName, UValAttributeComponent::IsLowHealth(SelfActor));
			BlackBoardComp->SetValueAsBool(IsFullHealthKey.SelectedKeyName, UValAttributeComponent::IsFullHealth(SelfActor));
		}
	}
}
