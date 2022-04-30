// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathSignature, AActor*, DeadActor);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENPAI_API UBaseHealthComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	// Sets default values for this component's properties
	UBaseHealthComponent();

	UPROPERTY(BlueprintAssignable)
	FDeathSignature OnDeath;
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Health")
	float CurrentHealth;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void ReceiveDamage(float Damage);

	UFUNCTION()
	void DamageActor(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void KillActor(AActor* DeadActor);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
		
};
