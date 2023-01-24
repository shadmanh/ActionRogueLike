// Fill out your copyright notice in the Description page of Project Settings.


#include "ValActionComponent.h"
#include "ValAction.h"

UValActionComponent::UValActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UValActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<UValAction> ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void UValActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void UValActionComponent::AddAction(TSubclassOf<UValAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UValAction* NewAction = NewObject<UValAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool UValActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UValAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool UValActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (UValAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}

