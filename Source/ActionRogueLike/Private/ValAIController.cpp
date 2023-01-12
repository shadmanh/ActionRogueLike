// Fill out your copyright notice in the Description page of Project Settings.


#include "ValAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AValAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign Behaviour tree in your AI Controller.")))
	{
		RunBehaviorTree(BehaviorTree);
	}

	/*
	APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (MyPawn)
	{
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}
	*/

}
