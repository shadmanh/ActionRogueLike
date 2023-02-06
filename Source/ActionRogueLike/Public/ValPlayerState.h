// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ValPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API AValPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AValPlayerState();
	
	void UpdateCredits(int CreditAmount);

	int GetCredits();

	bool DeductCredits(int CreditAmount);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	int Credits;
	
};
