// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <GameplayTagContainer.h>
#include "ValMagicProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class UAudioComponent;
class UCameraShake;
class UValActionEffect;

UCLASS()
class ACTIONROGUELIKE_API AValMagicProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AValMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag ParryTag;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UValActionEffect> BurningActionClass;

	UFUNCTION()
	virtual void OnActorOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* EffectComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* LoopingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* ImpactSound;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> ShakeEffect;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
