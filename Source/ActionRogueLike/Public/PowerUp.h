// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Collectible.h"
#include "PowerUp.generated.h"

class UValAction;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API APowerUp : public ACollectible
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UValAction> ActionToGrant;

public:		
	APowerUp();

	virtual bool DoEffect(AValCharacter* Player) override;
};
