// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BaseWeaponComponent.generated.h"


UCLASS( Abstract, Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENPAI_API UBaseWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseWeaponComponent();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void Attack();

	bool CanAttack();

protected:

	UPROPERTY(EditAnywhere)
	float CooldownTime = 1;

	FTimerHandle CooldownResetTimer;

	bool bCanAttack;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void ResetCooldown();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
