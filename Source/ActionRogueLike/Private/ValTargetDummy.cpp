// Fill out your copyright notice in the Description page of Project Settings.


#include "ValTargetDummy.h"
#include <Components/StaticMeshComponent.h>
#include "ValAttributeComponent.h"

// Sets default values
AValTargetDummy::AValTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	AttributeComp = CreateDefaultSubobject<UValAttributeComponent>("AttributeComp");

	// Trigger when health is changed (damage/healing)
	AttributeComp->OnHealthChanged.AddDynamic(this, &AValTargetDummy::onHealthChanged);

}

void AValTargetDummy::onHealthChanged(AActor* InstigatorActor, UValAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		MeshComp->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
}

