// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValGameplayInterface.h"
#include "ValCharacter.h"
#include "Collectible.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ACollectible : public AActor, public IValGameplayInterface
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ACollectible();

protected:
	UPROPERTY(VisibleAnywhere, Replicated)
	bool IsActive = true;

	UPROPERTY(VisibleAnywhere, Replicated)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditDefaultsOnly)
	float RespawnTime;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual bool DoEffect(AValCharacter* Player);

	UFUNCTION()
	void SetActive(bool Flag);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastInteractSuccessful();
};
