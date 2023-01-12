// Fill out your copyright notice in the Description page of Project Settings.


#include "ValAttributeComponent.h"

// Sets default values for this component's properties
UValAttributeComponent::UValAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
}

UValAttributeComponent* UValAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UValAttributeComponent> (FromActor->GetComponentByClass(UValAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool UValAttributeComponent::IsActorAlive(AActor* Actor)
{
	UValAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

bool UValAttributeComponent::IsLowHealth(AActor* Actor)
{
	UValAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->Health <= AttributeComp->HealthMax/2.0f;
	}

	return false;
}

bool UValAttributeComponent::IsFullHealth(AActor* Actor)
{
	UValAttributeComponent* AttributeComp = GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->Health == AttributeComp->HealthMax;
	}

	return false;
}

bool UValAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UValAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	Health = FMath::Clamp(Health+Delta, 0.0f , HealthMax);

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
		
	UE_LOG(LogTemp, Log, TEXT("Health is % f"), Health);
	
	return true;
}

bool UValAttributeComponent::IsMaxHealth()
{
	return Health == HealthMax;
}
