// Fill out your copyright notice in the Description page of Project Settings.


#include "ValPlayerState.h"

AValPlayerState::AValPlayerState()
{
	Credits = 0;
}

void AValPlayerState::UpdateCredits(int NumCredits)
{
	Credits += NumCredits;
}

int AValPlayerState::GetCredits()
{
	return Credits;
}

bool AValPlayerState::DeductCredits(int CreditAmount)
{
	if (Credits >= CreditAmount)
	{
		Credits -= CreditAmount;
		return true;
	}
	return false;
}