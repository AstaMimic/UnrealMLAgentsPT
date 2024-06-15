// Fill out your copyright notice in the Description page of Project Settings.


#include "DRLClientComponent.h"

// Sets default values for this component's properties
UDRLClientComponent::UDRLClientComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UDRLClientComponent::BeginPlay()
{
	Super::BeginPlay();
	UDRLAcademy* Academy = UDRLAcademy::GetInstance();
}
