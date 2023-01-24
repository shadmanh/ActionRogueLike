// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ValAICharacter.generated.h"

class UPawnSensingComponent;
class UValAttributeComponent;
class UUserWidget;
class UValWorldUserWidget;

UCLASS()
class ACTIONROGUELIKE_API AValAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AValAICharacter();

protected:

	UValWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	int CreditForKill;
	
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UValAttributeComponent* OwningComp,
		float NewHealth, float Delta);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UValAttributeComponent* AttributeComp;
	
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

public:

	UFUNCTION()
	int GetCreditForKill();
};
