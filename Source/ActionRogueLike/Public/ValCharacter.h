// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ValCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UParticleSystem;
class UValInteractionComponent;
class UAnimMontage;
class UValAttributeComponent;
class UValActionComponent;

UCLASS()
class ACTIONROGUELIKE_API AValCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	// How far to trace before giving up on finding a hit
	float FallOffDistance = 10000.f;

public:
	// Sets default values for this character's properties
	AValCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UValInteractionComponent* InteractionComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UValAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UValActionComponent* ActionComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	FVector GetLocationBeingLookedAt();
	void PrimaryAttack();
	void SecondaryAttack();
	void SprintStart();
	void SprintStop();	

	void PrimaryInteract();

	void TeleportAbility();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UValAttributeComponent* OwningComp,
		float NewHealth, float Delta);

	virtual void PostInitializeComponents() override;

	virtual FVector GetPawnViewLocation() const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UValAttributeComponent* GetAttributeComp();

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
	
	UFUNCTION(BlueprintCallable)
	void GrantCredits(int CreditAmount);

	UFUNCTION(BlueprintCallable)
	int GetCredits();

	UFUNCTION(BlueprintCallable)
	bool DeductCredits(int CreditAmount);

};
