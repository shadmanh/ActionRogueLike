// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ValInteractionComponent.generated.h"

class UValWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UValInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UValInteractionComponent();

	void PrimaryInteract();

protected:

	// Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgment was received
	// Unreliable - Not guaranteed, packet can get lost and won't retry.

	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusedActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UValWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UValWorldUserWidget* DefaultWidgetInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
