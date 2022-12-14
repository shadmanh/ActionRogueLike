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

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;

};
