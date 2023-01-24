// Fill out your copyright notice in the Description page of Project Settings.


#include "ValAttributeComponent.h"
#include "ValGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("val,DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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

bool UValAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}

bool UValAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UValAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;

	Health = FMath::Clamp(Health+Delta, 0.0f , HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);
		
	UE_LOG(LogTemp, Log, TEXT("Health is % f"), Health);
	
	// Died
	if (ActualDelta < 0.0f && Health == 0.0f)
	{
		AValGameModeBase* GM = GetWorld()->GetAuthGameMode<AValGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return true;
}

bool UValAttributeComponent::IsMaxHealth()
{
	return Health == HealthMax;
}
