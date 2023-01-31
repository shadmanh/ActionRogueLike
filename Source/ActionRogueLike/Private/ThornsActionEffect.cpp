// Fill out your copyright notice in the Description page of Project Settings.


#include "ThornsActionEffect.h"
#include "ValAttributeComponent.h"
#include "ValActionComponent.h"

UThornsActionEffect::UThornsActionEffect()
{
	ReflectDamagePercentage = 0.5f;
	Duration = 0.f;
	Period = 0.f;
}

void UThornsActionEffect::SetReflectDamagePercentage(float Percentage)
{
	ReflectDamagePercentage = Percentage;
}


void UThornsActionEffect::ExecuteEffect_Implementation(AActor* InstigatorActor, UValAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	UValAttributeComponent* AttributeComp = Cast<UValAttributeComponent>(InstigatorActor->GetComponentByClass(UValAttributeComponent::StaticClass()));

	if (AttributeComp)
	{
		AttributeComp->ApplyHealthChange(OwningComp->GetOwner(), (int)(Delta * ReflectDamagePercentage));
	}
}

// The Instigator gets damaged
// The owner is the one who deals the damage in the case of thorns
void UThornsActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	
	UValAttributeComponent* AttributeComp = Cast<UValAttributeComponent>(Instigator->GetComponentByClass(UValAttributeComponent::StaticClass()));

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &UThornsActionEffect::ExecuteEffect);
	}
}

void UThornsActionEffect::StopAction_Implementation(AActor* Instigator)
{
	UValAttributeComponent* AttributeComp = Cast<UValAttributeComponent>(Instigator->GetComponentByClass(UValAttributeComponent::StaticClass()));

	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &UThornsActionEffect::ExecuteEffect);
	}


	Super::StopAction_Implementation(Instigator);

}
