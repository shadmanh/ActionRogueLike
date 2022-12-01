// Fill out your copyright notice in the Description page of Project Settings.


#include "ValAttributeComponent.h"

// Sets default values for this component's properties
UValAttributeComponent::UValAttributeComponent()
{
	Health = 100;
}

bool UValAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;

	onHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}