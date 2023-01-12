// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "ValAttributeComponent.h"
#include "ValCharacter.h"

// Sets default values
AHealthPotion::AHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthPotion::SetActive()
{
	IsActive = true;
	BaseMesh->SetVisibility(true);
}

void AHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (IsActive)
	{
		AValCharacter* PlayerCharacter = Cast<AValCharacter>(InstigatorPawn);
		if (PlayerCharacter)
		{
			UValAttributeComponent* AttComp = PlayerCharacter->GetAttributeComp();
			if (AttComp && !AttComp->IsMaxHealth()) {
				AttComp->ApplyHealthChange(this, HealAmount);
				IsActive = false;
				BaseMesh->SetVisibility(false);
				FTimerHandle TimerHandle_Health;
				GetWorldTimerManager().SetTimer(TimerHandle_Health, this,
					&AHealthPotion::SetActive, 10.f);
			}
		}
	}
}

