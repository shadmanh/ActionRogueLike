// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"
#include <Net/UnrealNetwork.h>

// Sets default values
ACollectible::ACollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	RespawnTime = 10.0f;
	
	SetReplicates(true);

}

// Called when the game starts or when spawned
void ACollectible::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectible::Interact_Implementation(APawn* InstigatorPawn)
{
	if (IsActive)
	{
		AValCharacter* PlayerCharacter = Cast<AValCharacter>(InstigatorPawn);
		if (PlayerCharacter)
		{

			if (DoEffect(PlayerCharacter))
			{
				MulticastInteractSuccessful();
			}
		}
	}
}

bool ACollectible::DoEffect(AValCharacter* Player)
{
	return false;
}

void ACollectible::SetActive(bool Flag)
{
	IsActive = Flag;
	BaseMesh->SetVisibility(Flag);
}

void ACollectible::MulticastInteractSuccessful_Implementation()
{
	SetActive(false);
	FTimerHandle TimerHandle_Health;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "SetActive", true);
	GetWorldTimerManager().SetTimer(TimerHandle_Health, Delegate, RespawnTime, false);
}

void ACollectible::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollectible, IsActive);
	DOREPLIFETIME(ACollectible, BaseMesh);
}
