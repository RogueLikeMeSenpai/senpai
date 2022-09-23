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

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<ABaseWeapon> WeaponType;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UAnimMontage* PrimaryAttackMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float PrimaryAttackPlayRate = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UAnimMontage* SecondaryAttackMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float SecondaryAttackPlayRate = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	EWeaponSocket HandSocket;
	

};
