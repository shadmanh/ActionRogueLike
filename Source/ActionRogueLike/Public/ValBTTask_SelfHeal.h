// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ValBTTask_SelfHeal.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UValBTTask_SelfHeal : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	float RegenRate;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector IsFullHealthKey;

		
public:

	UValBTTask_SelfHeal();
	
};
