// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"

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
				IsActive = false;
				BaseMesh->SetVisibility(false);
				FTimerHandle TimerHandle_Health;
				GetWorldTimerManager().SetTimer(TimerHandle_Health, this,
					&ACollectible::SetActive, RespawnTime);
				UE_LOG(LogTemp, Log, TEXT("The player has %i credits."), PlayerCharacter->GetCredits());
			}
		}
	}
}

bool ACollectible::DoEffect(AValCharacter* Player)
{
	return false;
}

void ACollectible::SetActive()
{
	IsActive = true;
	BaseMesh->SetVisibility(true);
}
