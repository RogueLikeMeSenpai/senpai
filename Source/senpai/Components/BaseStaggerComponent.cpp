// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseStaggerComponent.h"

// Sets default values for this component's properties
UBaseStaggerComponent::UBaseStaggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBaseStaggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentStaggerValue = 0;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UBaseStaggerComponent::StaggerActor);
}

void UBaseStaggerComponent::StaggerActor(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser)
{
	// Check Damage Type here for stagger multiplier
	IncreaseStaggerValue(Damage);
}

void UBaseStaggerComponent::IncreaseStaggerValue(float Amount)
{
	CurrentStaggerValue += Amount;
	CurrentStaggerValue = FMath::Clamp(CurrentStaggerValue, 0.f, MaxStaggerValue);
	OnStaggerValueIncreased.Broadcast(GetOwner());

	if (CurrentStaggerValue > MaxStaggerValue)
	{
		OnFullStagger.Broadcast(GetOwner());
		CurrentStaggerValue = 0;
	}
}

void UBaseStaggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentStaggerValue -= DeteriorationRate * DeltaTime;
	CurrentStaggerValue  = FMath::Clamp(CurrentStaggerValue,0,MaxStaggerValue);

}