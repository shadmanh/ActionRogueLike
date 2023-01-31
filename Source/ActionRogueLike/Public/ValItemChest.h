// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValGameplayInterface.h"
#include "ValItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API AValItemChest : public AActor, public IValGameplayInterface
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(VisibleAnywhere)
	float TargetPitch;

	// Sets default values for this actor's properties
	AValItemChest();

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly) // RepNotify
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

};
