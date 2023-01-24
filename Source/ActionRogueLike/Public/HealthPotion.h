// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectible.h"
#include "HealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AHealthPotion : public ACollectible
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPotion();

protected:
	UPROPERTY(EditDefaultsOnly)
	float HealAmount;
	
	UPROPERTY(EditDefaultsOnly)
	int CostToUse;

public:		
	virtual bool DoEffect(AValCharacter* Player) override;

};
