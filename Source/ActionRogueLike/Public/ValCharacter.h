// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ValCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UValInteractionComponent;
class UAnimMontage;
class UValAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API AValCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> DamageProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> GravityProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandle_Attack;

	FTimerDelegate TimerDel;

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	FVector GetLocationBeingLookedAt();
	void PrimaryAttack();
	void SecondaryAttack();
	void PreAttack();
	
	UFUNCTION()
	void Attack_TimeElapsed(UClass* ChosenProjectileClass);

	void PrimaryInteract();

	void TeleportAbility();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
