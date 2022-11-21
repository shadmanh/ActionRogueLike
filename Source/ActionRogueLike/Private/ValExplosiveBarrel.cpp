// Fill out your copyright notice in the Description page of Project Settings.


#include "ValExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include <DrawDebugHelpers.h>

// Sets default values
AValExplosiveBarrel::AValExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComp->SetCollisionProfileName("PhysicsActor");
	StaticMeshComp->SetSimulatePhysics(true);
	RootComponent = StaticMeshComp;
	
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForceComp->SetupAttachment(StaticMeshComp);
	FVector RelativeLocation;
	RelativeLocation.Z = 70.0f;
	RadialForceComp->SetRelativeLocation(RelativeLocation);
	RadialForceComp->Radius = 700.0f;
	RadialForceComp->ImpulseStrength = 2000.0f;
	RadialForceComp->bImpulseVelChange = true;
}

void AValExplosiveBarrel::PostInitializeComponents()
{
	
	Super::PostInitializeComponents();
	StaticMeshComp->OnComponentHit.AddDynamic(this, &AValExplosiveBarrel::OnCompHit);
}

// Called when the game starts or when spawned
void AValExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AValExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AValExplosiveBarrel::OnCompHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComp->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"));

	// logs: "OtherActor: MyActor_1, at gametime: 124.4"
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"),
		*GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
}

