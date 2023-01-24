// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"

ACoin::ACoin()
{
	CreditAmount = 50.f;
}

bool ACoin::DoEffect(AValCharacter* Player)
{
	Player->GrantCredits(CreditAmount);
	return true;
}
