// Fill out your copyright notice in the Description page of Project Settings.


#include "ValPlayerController.h"

void AValPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}
