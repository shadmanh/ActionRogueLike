// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ValAICharacter.generated.h"

class UPawnSensingComponent;
class UValAttributeComponent;
class UValActionComponent;
class UUserWidget;
class UValWorldUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetPawnChanged,
APawn*, Player);

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

	AActor* TargetActor;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UValAttributeComponent* OwningComp,
		float NewHealth, float Delta);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UValAttributeComponent* AttributeComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UValActionComponent* ActionComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UValWorldUserWidget> SpottedPlayerWidgetClass;

	UPROPERTY()
	UValWorldUserWidget* SpottedPlayerWidgetInstance;
	
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

public:

	UFUNCTION()
	int GetCreditForKill();
	
	UPROPERTY(BlueprintAssignable)
	FOnTargetPawnChanged OnTargetPawnChanged;
};
