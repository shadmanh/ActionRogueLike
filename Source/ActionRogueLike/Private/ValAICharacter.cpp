// Fill out your copyright notice in the Description page of Project Settings.


#include "ValAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>
#include "ValAttributeComponent.h"
#include "BrainComponent.h"
#include "ValWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ValCharacter.h"
#include "ValActionComponent.h"

// Sets default values
AValAICharacter::AValAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<UValAttributeComponent>("AttributeComp");
	
	ActionComp = CreateDefaultSubobject<UValActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	TimeToHitParamName = "TimeToHit";

	CreditForKill = 10;
}

void AValAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AValAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &AValAICharacter::OnHealthChanged);
}

void AValAICharacter::OnHealthChanged(AActor* InstigatorActor, UValAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("Bot health is %f"), NewHealth);
		
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{	
			ActiveHealthBar = CreateWidget<UValWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);

		// Died
		if (NewHealth <= 0.0f)
		{
			// stop bt
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			//ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			//set lifespan
			SetLifeSpan(10.0f);
		}
	}

}

void AValAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
		TargetActor = NewTarget;
	}
}

void AValAICharacter::OnPawnSeen(APawn* Pawn)
{
	if (TargetActor != Pawn) {
		SetTargetActor(Pawn);
	
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr,
		FColor::White, 4.0f, true);

		OnTargetPawnChanged.Broadcast(Pawn);
		
		if (SpottedPlayerWidgetInstance == nullptr && ensure(SpottedPlayerWidgetClass))
		{
			SpottedPlayerWidgetInstance = CreateWidget<UValWorldUserWidget>(GetWorld(), SpottedPlayerWidgetClass);
			SpottedPlayerWidgetInstance->AttachedActor = this;
			SpottedPlayerWidgetInstance->AddToViewport();

		}

	}
}

int AValAICharacter::GetCreditForKill()
{
	return CreditForKill;
}

