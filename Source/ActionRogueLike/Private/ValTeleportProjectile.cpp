// Fill out your copyright notice in the Description page of Project Settings.


#include "ValTeleportProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AValTeleportProjectile::AValTeleportProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereComp->OnComponentHit.AddDynamic(this, &AValTeleportProjectile::OnComponentHit);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleProjectile(TEXT(
		"/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_Projectile"));
	ParticleProjectileFX = ParticleProjectile.Object;
	EffectComp->SetTemplate(ParticleProjectileFX);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleHit(TEXT(
		"/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Primary/FX/P_Gideon_Primary_HitWorld"));
	ParticleHitFX = ParticleHit.Object;

	MovementComp->ProjectileGravityScale = 0.0f;
}

void AValTeleportProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		Detonate();
	}
}

// Called when the game starts or when spawned
void AValTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this,
		&AValTeleportProjectile::Detonate, 0.4f);
	
}

void AValTeleportProjectile::Detonate()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Projectile);

	if (ParticleHitFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleHitFX, EffectComp->GetComponentTransform());
	}

	MovementComp->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(TimerHandle_Projectile, this,
		&AValTeleportProjectile::Teleport, 0.4f);

}

void AValTeleportProjectile::Teleport()
{
	if (ensure(GetInstigator())) {
		GetInstigator()->TeleportTo(this->GetActorLocation(), GetInstigator()->GetActorRotation());
	}

	Destroy();
}

// Called every frame
void AValTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

