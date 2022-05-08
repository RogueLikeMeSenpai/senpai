// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class SENPAI_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

protected:

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* AttackEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;

	UFUNCTION(BlueprintCallable)
	void PlayAttackEffects(FTransform Location);

	void ResetCooldown();
	
	UPROPERTY(EditDefaultsOnly)
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
