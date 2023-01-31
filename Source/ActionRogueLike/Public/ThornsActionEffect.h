// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ValActionEffect.h"
#include "ThornsActionEffect.generated.h"

class UValAttributeComponent;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UThornsActionEffect : public UValActionEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "ThornActionEffects")
	float ReflectDamagePercentage;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecuteEffect(AActor* InstigatorActor, UValAttributeComponent* OwningComp, float NewHealth, float Delta);
	
public:

	UThornsActionEffect();

	void StartAction_Implementation(AActor* Instigator) override;

	void StopAction_Implementation(AActor* Instigator) override;
	
	void SetReflectDamagePercentage(float Percentage);

};
