// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseStaminaComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENPAI_API UBaseStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseStaminaComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaximumStamina = 100;

	//Regeneration Rate per Second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RegenerationRate = 10;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float CurrentStamina;	


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Tries to use amount of stamina
	//returns false if CurrentStamina too low
	//returns true on success
	UFUNCTION(BlueprintCallable)
	bool UseStamina(float Amount);

	UFUNCTION(BlueprintCallable)
	float GetCurrentStamina() const;

	UFUNCTION(BlueprintCallable)
	bool HasEnoughStaminaFor(float Amount);
		
};
