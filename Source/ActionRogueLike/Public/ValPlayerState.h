// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ValPlayerState.generated.h"

class UValSaveGame;

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

	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(UValSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(UValSaveGame* SaveObject);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	int32 Credits;
	
};
