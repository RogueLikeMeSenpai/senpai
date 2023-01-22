// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp =CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	//MeshComp->SetupAttachment(RootComponent);
	RootComponent = MeshComp;

	PrimaryCooldown = 1;
	PrimaryBaseDamage = 50;
	bCanUsePrimaryAttack = true;

	SecondaryCooldown = 2;
	SecondaryBaseDamage = 100;
	bCanUseSecondaryAttack = true;
}

void ABaseWeapon::PlayAttackEffects()
{
	if (AttackEffect) {
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(AttackEffect, MeshComp, NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
		// Parameters can be set like this (see documentation for further info) - the names and type must match the user exposed parameter in the Niagara System
		//NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);
	}

}
// Should always be calles last to set cooldown timer
void ABaseWeapon::UsePrimaryAttack_Implementation()
{
	if(!bCanUsePrimaryAttack) return;

	//UE_LOG(LogTemp,Warning,TEXT("UsePrimaryAttack Parent called"));
	bCanUsePrimaryAttack = false;
	GetWorldTimerManager().SetTimer(TimerHandle_ResetTimer,this,&ABaseWeapon::ResetPrimaryCooldown,PrimaryCooldown,false);
	//PlayAttackEffects(FTransform::Identity);
}

void ABaseWeapon::UseSecondaryAttack_Implementation()
{
	if(!bCanUseSecondaryAttack) return;

	bCanUseSecondaryAttack = false;

	GetWorldTimerManager().SetTimer(TimerHandle_SecondaryResetTimer, this , &ABaseWeapon::ResetSecondaryCooldown,SecondaryCooldown,false);
}

void ABaseWeapon::EndPrimaryAttack_Implementation()
{
	
}

void ABaseWeapon::EndSecondaryAttack_Implementation()
{
	
}


void ABaseWeapon::ResetPrimaryCooldown()
{
	bCanUsePrimaryAttack = true;
}

void ABaseWeapon::ResetSecondaryCooldown()
{
	bCanUseSecondaryAttack = true;
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::TriggerDamageDealt(float Damage, AActor* DamagedActor)
{	
	OnDealtDamage.Broadcast(Damage, DamagedActor, this->GetClass());
}

//void ABaseWeapon::DealtDamage()
//{
//	UE_LOG(LogTemp, Display, TEXT("Damage Dealt"));
//}

void ABaseWeapon::AttachedToParent_Implementation()
{

}