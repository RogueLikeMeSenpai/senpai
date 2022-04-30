// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHealthComponent.h"


// Sets default values for this component's properties
UBaseHealthComponent::UBaseHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UBaseHealthComponent::DamageActor);
	OnDeath.AddDynamic(this, &UBaseHealthComponent::KillActor);
}


void UBaseHealthComponent::DamageActor(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	ReceiveDamage(Damage);
	UE_LOG(LogTemp, Warning, TEXT("Got %f Damage"), Damage);
}

void UBaseHealthComponent::KillActor(AActor* DeadActor)
{
	UE_LOG(LogTemp, Warning, TEXT("%s died!"), *GetName());
	GetOwner()->OnTakeAnyDamage.RemoveAll(this);
	//GetOwner()->Destroy();
}

// Called every frame
void UBaseHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UBaseHealthComponent::ReceiveDamage(float Damage)
{
	CurrentHealth -= Damage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("%s got %f damage"), *GetName(), Damage);
	if(CurrentHealth <= 0.f)
	{
		//Dead
		UE_LOG(LogTemp, Warning, TEXT("%s should die"), *GetName());
		OnDeath.Broadcast(GetOwner());
	}
}

