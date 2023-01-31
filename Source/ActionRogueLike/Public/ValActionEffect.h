// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValAction.h"
#include "ValActionEffect.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UValActionEffect : public UValAction
{
	GENERATED_BODY()

public:

	UValActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;

	// Time between 'ticks' to apply effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle PeriodHandle;
	FTimerHandle DurationHandle;

	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
