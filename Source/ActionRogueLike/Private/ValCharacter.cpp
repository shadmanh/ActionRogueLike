// Fill out your copyright notice in the Description page of Project Settings.


#include "ValCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ValInteractionComponent.h"
#include "ValAttributeComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>


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

void AValCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &AValCharacter::OnHealthChanged);
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
	TraceBegin = CameraLoc + (CameraRot.Vector() * 200);
	TraceEnd = CameraLoc + (CameraRot.Vector() * FallOffDistance);

	FCollisionShape Shape;
	Shape.SetSphere(20.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Object Query Parameters
	FCollisionObjectQueryParams ObjectsToHit;
	ObjectsToHit.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectsToHit.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectsToHit.AddObjectTypesToQuery(ECC_Pawn);

	const FName CameraTraceTag("CameraTrace");

	// additional trace parameters
	FCollisionQueryParams TraceParams(CameraTraceTag, true, NULL);
	TraceParams.bTraceComplex = true;

	GetWorld()->DebugDrawTraceTag = CameraTraceTag;

	//Re-initialize hit info
	FHitResult HitDetails = FHitResult(ForceInit);
	bool bIsHit = GetWorld()->SweepSingleByObjectType(HitDetails,
		TraceBegin, TraceEnd, FQuat::Identity, ObjectsToHit, Shape, TraceParams);

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
	FName HandSocketName = FName("Muzzle_01");
	FVector HandLocation = GetMesh()->GetSocketLocation(HandSocketName);
	//FVector HandLocation = FVector(0, 0, 500);
	FRotator HandRotation = GetMesh()->GetSocketRotation(HandSocketName);
	FTransform SpawnTM = FTransform(HandRotation, HandLocation);
	
	UGameplayStatics::SpawnEmitterAttached(ProjectileSpawnFX, GetMesh(), HandSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSpawnFX, SpawnTM);
	
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
		
		// The interact distance shouldn't be too large
		FallOffDistance = 1000.f;
		
		InteractionComp->PrimaryInteract(GetLocationBeingLookedAt());
	}
}

void AValCharacter::TeleportAbility()
{
	TimerDel.BindUFunction(this, FName("Attack_TimeElapsed"), TeleportProjectileClass);
	PreAttack();	
}

void AValCharacter::OnHealthChanged(AActor* InstigatorActor, UValAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		
		if (NewHealth <= 0.0f)
		{
			APlayerController* PC = Cast<APlayerController>(GetController());
			DisableInput(PC);
		}
	}
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

UValAttributeComponent* AValCharacter::GetAttributeComp()
{
	return AttributeComp;
}

