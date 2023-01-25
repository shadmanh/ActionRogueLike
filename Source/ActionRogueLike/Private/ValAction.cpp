// Fill out your copyright notice in the Description page of Project Settings.


#include "ValAction.h"
#include "ValActionComponent.h"

void UValAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	
	UValActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void UValAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	
	ensureAlways(bIsRunning);

	UValActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

UWorld* UValAction::GetWorld() const
{
	// Outer is set when creating action via NewObject<T>
	UActorComponent* Comp = Cast<UActorComponent>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}

	return nullptr;
}

bool UValAction::CanStart_Implementation(AActor* Instigator)
{
	if (IsRunning())
	{
		return false;
	}

	UValActionComponent* Comp = GetOwningComponent();

	return !(Comp->ActiveGameplayTags.HasAny(BlockedTags));

}

UValActionComponent* UValAction::GetOwningComponent() const
{
	return Cast<UValActionComponent>(GetOuter());
}

bool UValAction::IsRunning_Implementation() const
{
	return bIsRunning;
}

