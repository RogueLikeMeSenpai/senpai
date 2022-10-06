// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseStaminaComponent.h"

// Sets default values for this component's properties
UBaseStaminaComponent::UBaseStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentStamina = MaximumStamina;
	
}


// Called every frame
void UBaseStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentStamina+= RegenerationRate * DeltaTime;
	CurrentStamina = FMath::Clamp(CurrentStamina,0,MaximumStamina);
	
}

bool UBaseStaminaComponent::UseStamina(float Amount)
{
	if(CurrentStamina < Amount) return false;

	CurrentStamina -= Amount;
	return true;
}

float UBaseStaminaComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

