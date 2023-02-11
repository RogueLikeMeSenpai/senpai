// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeathSignature, AActor*, DeadActor, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangedSignature, AActor*, Owner, float, change);


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENPAI_API UBaseHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UBaseHealthComponent();

	UPROPERTY(BlueprintAssignable)
	FDeathSignature OnDeath;
	UPROPERTY(BlueprintAssignable)
	FHealthChangedSignature OnHealthChanged;
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	bool DestroyOwnerOnDeath = false;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void ReceiveDamage(float Damage, AActor* DamageCauser);

	UFUNCTION()
	void DamageActor(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void KillActor(AActor* DeadActor, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
		
};
