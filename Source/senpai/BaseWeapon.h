// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Delegates/Delegate.h"
#include "BaseWeapon.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDealtDamageSignature, float, Damage, AActor*, DamagedActor);

UCLASS()
class SENPAI_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	APawn* WeaponOwner;


	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FDealtDamageSignature OnDealtDamage;

protected:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UNiagaraSystem* AttackEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float BaseDamage;

	UFUNCTION(BlueprintCallable)
	void PlayAttackEffects();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TriggerDamageDealt(float Damage, AActor* DamagedActor);

	

	void ResetCooldown();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bCanAttack;

	
	
	FTimerHandle TimerHandle_TimeBetweenAttacks;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Attack();
	
	virtual void Tick(float DeltaTime) override;

};
