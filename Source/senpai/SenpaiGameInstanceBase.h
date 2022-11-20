// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SenpaiGameInstanceBase.generated.h"

USTRUCT(BlueprintType)
struct FSpawnDetails {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString  SpawnPointName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString  EnemyConfig;
};

/**
 * 
 */
UCLASS()
class SENPAI_API USenpaiGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentRun = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnDetails> SpawnHistory;

	UFUNCTION(BlueprintCallable)
	void ChangeLevel(int32 LevelId);

	UFUNCTION(BlueprintCallable)
	void RegisterSpawn(FString SpawnPointName, FString EnemyConfig);
};
