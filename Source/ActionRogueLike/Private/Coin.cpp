// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"

ACoin::ACoin()
{
	CreditAmount = 50.f;
}

bool ACoin::DoEffect(AValCharacter* Player)
{
	Player->GrantCredits(CreditAmount);
	UE_LOG(LogTemp, Log, TEXT("The player has %i credits."), Player->GetCredits());
	return true;
}
