// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"


UENUM(BlueprintType)
enum class EWeaponSocket: uint8{
	LEFT,
	RIGHT
};

/**
 * 
 */
UCLASS(BlueprintType)
class SENPAI_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Type")
	TSubclassOf<ABaseWeapon> WeaponType;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Primary")
	UAnimMontage* PrimaryAttackMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Primary")
	float PrimaryAttackPlayRate = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Primary")
	float PrimaryStaminaCost = 10;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Secondary")
	UAnimMontage* SecondaryAttackMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Secondary")
	float SecondaryAttackPlayRate = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Secondary")
	float SecondaryStaminaCost = 20;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Location")
	EWeaponSocket HandSocket;
};
