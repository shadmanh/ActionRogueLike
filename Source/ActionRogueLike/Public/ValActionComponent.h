// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <GameplayTagContainer.h>
#include "ValActionComponent.generated.h"

class UValAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UValActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void AddAction(AActor* Instigator, TSubclassOf<UValAction> ActionClass);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void RemoveAction(UValAction* ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

	UValActionComponent();

protected:
	
	// Granted abilities at game start
	UPROPERTY(EditAnywhere, Category = "Actions")
		TArray<TSubclassOf<UValAction>> DefaultActions;

	UPROPERTY()
	TArray<UValAction*> Actions;

	virtual void BeginPlay() override;


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsActionInside(UValAction* QueryAction);
	
};
