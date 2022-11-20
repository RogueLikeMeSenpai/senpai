// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <senpai/SenpaiGameInstanceBase.h>
#include "EnemyRemark_DataAsset.generated.h"



USTRUCT(BlueprintType)
struct FEnemyRemarkCondition {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinNumberOfMatches;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfSpawnsToCheck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BeginningFromHistoryIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MatchEnemyConfigRegex;
};

USTRUCT(Blueprinttype)
struct FEnemyRemark {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinRun = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxRun = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText remark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyRemarkCondition> Conditions;
};

/**
 * 
 */
UCLASS(BlueprintType)
class SENPAI_API UEnemyRemark_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UEnemyRemark_DataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyRemark> remarks;
	
	UFUNCTION(BlueprintCallable)
	TArray<FEnemyRemark> FindApplicableRemarks(USenpaiGameInstanceBase* gameInstance, FString SpawnPointName, FString EnemyConfig);

};
