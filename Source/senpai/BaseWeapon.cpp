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
	MeshComp->SetupAttachment(RootComponent);
	//RootComponent = MeshComp;

	Cooldown = 1;
	BaseDamage = 50;
	bCanAttack = true;
}

void ABaseWeapon::PlayAttackEffects(FTransform Location)
{
	if(!AttackEffect) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackEffect, Location);
}

void ABaseWeapon::Attack_Implementation()
{
	if(!bCanAttack) return;

	UE_LOG(LogTemp,Warning,TEXT("Attack Parent called"));
	bCanAttack = false;
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenAttacks,this,&ABaseWeapon::ResetCooldown,Cooldown,false);
	
}

void ABaseWeapon::ResetCooldown()
{
	bCanAttack = true;
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

