// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Delegates/Delegate.h"
#include "BaseWeapon.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDealtDamageSignature, float, Damage, AActor*, DamagedActor, UClass*, WeaponClass);

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
	TSubclassOf<UDamageType> PrimaryDamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> SecondaryDamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UNiagaraSystem* AttackEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UTexture2D* WeaponIcon;

	UFUNCTION(BlueprintCallable)
	void PlayAttackEffects();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TriggerDamageDealt(float Damage, AActor* DamagedActor);

	void ResetPrimaryCooldown();

	void ResetSecondaryCooldown();
	
	//###Primary attack variables###

	//Base damage for primary attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float PrimaryBaseDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PrimaryCooldown;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bCanUsePrimaryAttack;

	FTimerHandle TimerHandle_ResetTimer;

	//###Secondary attack variables###

	//Base damage for secondary attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float SecondaryBaseDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SecondaryCooldown;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bCanUseSecondaryAttack;

	FTimerHandle TimerHandle_SecondaryResetTimer;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	//Starts the primary attack
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void UsePrimaryAttack();

	//Starts the secondary attack
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void UseSecondaryAttack();

	//Used to finalize the attack (ending tracing or similar effects)
	//usage is optional
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void EndPrimaryAttack();
	//Used to finalize the attack (ending tracing or similar effects)
	//usage is optional
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void EndSecondaryAttack();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AttachedToParent();

	
	virtual void Tick(float DeltaTime) override;

};
