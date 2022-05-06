// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ChildActorComponent.h"
#include "Test_ChildActorComponent_AIModel.generated.h"

/**
 * 
 */
UCLASS()
class SENPAI_API UTest_ChildActorComponent_AIModel : public UChildActorComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "CoolCategory")
	void MySuperCoolFunction();

	UPROPERTY(EditAnywhere)
	int32 MyCoolVariable;
};
