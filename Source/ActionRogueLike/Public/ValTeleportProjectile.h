// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ValMagicProjectile.h"
#include <Particles/ParticleSystem.h>
#include "ValTeleportProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AValTeleportProjectile : public AValMagicProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AValTeleportProjectile();

protected:
	FTimerHandle TimerHandle_Projectile;
	
	UParticleSystem* ParticleProjectileFX;
	UParticleSystem* ParticleHitFX;

	UFUNCTION()
	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Detonate();
	virtual void Teleport();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
