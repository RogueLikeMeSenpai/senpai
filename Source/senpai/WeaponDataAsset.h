// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SENPAI_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseWeapon> WeaponType;
	UPROPERTY(EditAnywhere)
	UAnimMontage* PrimaryAttackMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* SecondaryAttackMontage;

	

};
