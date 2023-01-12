// Fill out your copyright notice in the Description page of Project Settings.



#include "ValBTTask_SelfHeal.h"
#include <AIController.h>
#include "../Public/ValAICharacter.h"
#include "../Public/ValAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UValBTTask_SelfHeal::UValBTTask_SelfHeal()
{
	RegenRate = 1.0f;
}

EBTNodeResult::Type UValBTTask_SelfHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		AValAICharacter* MyPawn = Cast<AValAICharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		UValAttributeComponent* AttributeComp = Cast<UValAttributeComponent>(MyPawn->GetComponentByClass(UValAttributeComponent::StaticClass()));

		AttributeComp->ApplyHealthChange(MyPawn, RegenRate);
	
		UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
		BlackBoardComp->SetValueAsBool(IsFullHealthKey.SelectedKeyName, UValAttributeComponent::IsFullHealth(MyPawn));
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

