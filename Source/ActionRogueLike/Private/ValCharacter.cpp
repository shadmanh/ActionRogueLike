// Fill out your copyright notice in the Description page of Project Settings.


#include "ValCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ValInteractionComponent.h"
#include "ValAttributeComponent.h"
#include <Kismet/KismetMathLibrary.h>


// Sets default values
AValCharacter::AValCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<UValInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<UValAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AValCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AValCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	AddMovementInput(ControlRot.Vector(), Value);
}

void AValCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// X = Forward (Red)
	// Y = Right (Green)
	// Z = Up (Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value);
}

FVector AValCharacter::GetLocationBeingLookedAt() {
		
	FVector TraceBegin;
	FVector TraceEnd;

	// get the camera view
	FVector CameraLoc = CameraComp->GetComponentLocation();
	FRotator CameraRot = CameraComp->GetComponentRotation();
	TraceBegin = CameraLoc;
	TraceEnd = CameraLoc + (CameraRot.Vector() * FallOffDistance);

	// Object Query Parameters
	FCollisionObjectQueryParams ObjectsToHit;
	ObjectsToHit.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectsToHit.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectsToHit.AddObjectTypesToQuery(ECC_Vehicle);
	ObjectsToHit.AddObjectTypesToQuery(ECC_Destructible);

	const FName CameraTraceTag("CameraTrace");

	// additional trace parameters
	FCollisionQueryParams TraceParams(CameraTraceTag, true, NULL);
	TraceParams.bTraceComplex = true;

	GetWorld()->DebugDrawTraceTag = CameraTraceTag;

	//Re-initialize hit info
	FHitResult HitDetails = FHitResult(ForceInit);
	bool bIsHit = GetWorld()->LineTraceSingleByObjectType(HitDetails,
		TraceBegin, TraceEnd, ObjectsToHit, TraceParams);	

	if (bIsHit)
	{
		return HitDetails.ImpactPoint;
	}
	else
	{
		return TraceEnd;
	}
}

void AValCharacter::PrimaryAttack()
{
	TimerDel.BindUFunction(this, FName("Attack_TimeElapsed"), DamageProjectileClass);
	PreAttack();
}

void AValCharacter::SecondaryAttack()
{
	TimerDel.BindUFunction(this, FName("Attack_TimeElapsed"), GravityProjectileClass);
	PreAttack();
}

void AValCharacter::PreAttack()
{	
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_Attack, TimerDel, 0.2f, false);
}

void AValCharacter::Attack_TimeElapsed(UClass* ChosenProjectileClass)
{
	if (ensure(ChosenProjectileClass))
	{
		// The teleport projectile travels a short distance before stopping, so its falloff
		// distance is much smaller
		FallOffDistance = ChosenProjectileClass == TeleportProjectileClass ? 700.f : 10000.f;

		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FVector TargetLocation = GetLocationBeingLookedAt();

		FRotator ProjectileRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, TargetLocation);

		FTransform SpawnTM = FTransform(ProjectileRot, HandLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		GetWorld()->SpawnActor<AActor>(ChosenProjectileClass, SpawnTM, SpawnParams);
	}
}

void AValCharacter::PrimaryInteract()
{
	if (InteractionComp) {
		InteractionComp->PrimaryInteract();
	}
}

void AValCharacter::TeleportAbility()
{
	TimerDel.BindUFunction(this, FName("Attack_TimeElapsed"), TeleportProjectileClass);
	PreAttack();	
}

// Called every frame
void AValCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void AValCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AValCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AValCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AValCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("SecondaryAttack", IE_Pressed, this, &AValCharacter::SecondaryAttack);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AValCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("MovementAbility", IE_Pressed, this, &AValCharacter::TeleportAbility);
}

