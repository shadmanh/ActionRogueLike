// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "ValAction.h"
#include "ValCharacter.h"
#include "ValActionComponent.h"

APowerUp::APowerUp()
{

}

bool APowerUp::DoEffect(AValCharacter* Player)
{
	UValActionComponent* ActionComp = Cast<UValActionComponent>(Player->GetComponentByClass(UValActionComponent::StaticClass()));
	if (ActionComp->IsActionInside(NewObject<UValAction>(this, ActionToGrant))) 
	{
		return false;
	}
	ActionComp->AddAction(Player, ActionToGrant);

	return true;
}
