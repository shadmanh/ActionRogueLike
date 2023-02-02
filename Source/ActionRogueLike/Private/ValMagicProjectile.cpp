// Fill out your copyright notice in the Description page of Project Settings.


#include "ValMagicProjectile.h"
#include "Camera/CameraShake.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ValAttributeComponent.h"
#include <Kismet/GameplayStatics.h>
#include "ValGameplayFunctionLibrary.h"
#include "ValActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ValActionEffect.h"

// Sets default values
AValMagicProjectile::AValMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AValMagicProjectile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject <UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	LoopingSound = CreateDefaultSubobject <UAudioComponent>("LoopingSound");
	LoopingSound->SetupAttachment(RootComponent);
	ImpactSound = CreateDefaultSubobject <UAudioComponent>("ImpactSound");

	DamageAmount = 20.f;
	
	SetReplicates(true);
}

void AValMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		UValActionComponent* ActionComp = Cast<UValActionComponent>(OtherActor->GetComponentByClass(UValActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity = -MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		if (UValGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor,
			DamageAmount, SweepResult))
		{

			if (ActionComp && HasAuthority())
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}

			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound->Sound, GetActorLocation());

			UGameplayStatics::PlayWorldCameraShake(this, ShakeEffect, GetActorLocation(), 0, 1000.f);

			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void AValMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AValMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

