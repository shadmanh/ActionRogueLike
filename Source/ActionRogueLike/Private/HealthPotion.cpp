// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "ValAttributeComponent.h"
#include "ValCharacter.h"

// Sets default values
AHealthPotion::AHealthPotion()
{
	HealAmount = 50.f;
	CostToUse = 15;
}

bool AHealthPotion::DoEffect(AValCharacter* Player)
{
	UValAttributeComponent* AttComp = Player->GetAttributeComp();

	if (AttComp && !AttComp->IsMaxHealth() && Player->DeductCredits(CostToUse)) {
		AttComp->ApplyHealthChange(this, HealAmount);
		return true;
	}
	
	return false;
}

