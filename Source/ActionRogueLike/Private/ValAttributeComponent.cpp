// Fill out your copyright notice in the Description page of Project Settings.


#include "ValAttributeComponent.h"
#include "ValGameModeBase.h"
#include <Net/UnrealNetwork.h>

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("val,DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

// Sets default values for this component's properties
UValAttributeComponent::UValAttributeComponent()
{
	HealthMax = 100;
	Health = HealthMax;
	RageMax = 100;
	Rage = 0;
	RageDamageRatio = 1;
	BlackholeRageCost = -30;

	SetIsReplicatedByDefault(true);
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
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;
	
	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, Delta);
		}
		
		// Died
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			AValGameModeBase* GM = GetWorld()->GetAuthGameMode<AValGameModeBase>();
			if (GM)
			{
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}	

	return ActualDelta != 0;
}

bool UValAttributeComponent::IsMaxHealth()
{
	return Health == HealthMax;
}

bool UValAttributeComponent::ApplyRageChange(float Delta)
{
	if (Delta < 0) {
		Delta *= RageDamageRatio;
	}

	if (Delta + Rage < 0)
	{
		return false;
	}

	float OldRage = Rage;

	float NewRage = FMath::Clamp(Rage+Delta, 0.0f , RageMax);

	float ActualDelta = NewRage - OldRage;
	
	// Is Server?
	if (GetOwner()->HasAuthority())
	{
		Rage = NewRage;

		if (ActualDelta != 0.0f)
		{
			MulticastRageChanged(Rage, ActualDelta);
		}
		
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Rage is %f"), Rage));
	}	
	
	return ActualDelta != 0;

}

float UValAttributeComponent::GetBlackholeRageCost()
{
	return BlackholeRageCost;
}

void UValAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UValAttributeComponent, Health);
	DOREPLIFETIME(UValAttributeComponent, HealthMax);
}

void UValAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UValAttributeComponent::MulticastRageChanged_Implementation(float NewRage, float Delta)
{
	OnRageChanged.Broadcast(this, NewRage, Delta);

	// FIX THIS, clients aren't allowed to use black holes anymore even if they have enough rage
}

float UValAttributeComponent::GetRageDamageRatio()
{
	return RageDamageRatio;
}

