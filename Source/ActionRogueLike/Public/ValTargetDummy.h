// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValTargetDummy.generated.h"

class UValAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AValTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AValTargetDummy();

protected:
	UPROPERTY(VisibleAnywhere)
	UValAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void onHealthChanged(AActor* InstigatorActor, UValAttributeComponent* OwningComp,
		float NewHealth, float Delta);

public:	

};
