// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectible.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ACoin : public ACollectible
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float CreditAmount;

public:		
	ACoin();

	virtual bool DoEffect(AValCharacter* Player) override;
};
