// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Test_Actor_AIModel.generated.h"

UCLASS()
class SENPAI_API ATest_Actor_AIModel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATest_Actor_AIModel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "CoolCategory")
	void MyCoolFunction();

	UPROPERTY(EditAnywhere)
	int32 MyCoolVariable;

};
