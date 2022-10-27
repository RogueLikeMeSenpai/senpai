// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStaggerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaggerSignature, AActor *, StaggeredActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaggerChangedSignature, AActor *, StaggeredActor);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SENPAI_API UBaseStaggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBaseStaggerComponent();

	UPROPERTY(BlueprintAssignable)
	FStaggerSignature OnFullStagger;
	UPROPERTY(BlueprintAssignable)
	FStaggerChangedSignature OnStaggerValueIncreased;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stagger")
	float MaxStaggerValue = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Stagger")
	float CurrentStaggerValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stagger")
	float DeteriorationRate = 1;

	UFUNCTION()
	void StaggerActor(AActor *DamagedActor, float Damage, const class UDamageType *DamageType, class AController *InstigatedBy, AActor *DamageCauser);

	void IncreaseStaggerValue(float Amount);

	public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
