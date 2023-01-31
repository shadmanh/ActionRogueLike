// Fill out your copyright notice in the Description page of Project Settings.


#include "ValActionComponent.h"
#include "ValAction.h"

UValActionComponent::UValActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UValActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);

	SetIsReplicatedByDefault(true);
}

void UValActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<UValAction> ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void UValActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

bool UValActionComponent::IsActionInside(UValAction* QueryAction)
{
	for (auto CurrentAction : Actions)
	{
		if (CurrentAction == QueryAction) {
			return true;
		}
	}

	return false;
}

void UValActionComponent::AddAction(AActor* Instigator, TSubclassOf<UValAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	UValAction* NewAction = NewObject<UValAction>(this, ActionClass);
	if (ensure(NewAction))
	{
		if (IsActionInside(NewAction))
		{
			return;
		}

		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}

void UValActionComponent::RemoveAction(UValAction* ActionToRemove)
{
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}

	Actions.Remove(ActionToRemove);

}

bool UValActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (UValAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
			}

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
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

