// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRemark_DataAsset.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>



UEnemyRemark_DataAsset::UEnemyRemark_DataAsset()
{
}

TArray<FEnemyRemark> UEnemyRemark_DataAsset::FindApplicableRemarks(USenpaiGameInstanceBase* gameInstance, FString SpawnPointName, FString EnemyConfig)
{
	
	if (gameInstance == nullptr)
		return TArray<FEnemyRemark>();


	auto globalSpawnHistory = gameInstance->SpawnHistory;

	TArray<FEnemyRemark> applicableRemarks;
	for (const auto& remark: remarks)
	{
		if (
			(remark.MinRun >= 0 && remark.MinRun > gameInstance->CurrentRun) ||
			(remark.MaxRun >= 0 && remark.MaxRun < gameInstance->CurrentLevel)
			)
		{
			continue;
		}
			
		bool isApplicable = true;

		for (const auto& condition : remark.Conditions)
		{
			const FRegexPattern regexPattern(condition.MatchEnemyConfigRegex);			
			int32 numberOfMatches = 0;
			
			TArray<FSpawnDetails> spawnHistory;
			for (const auto& spawnDetail : globalSpawnHistory) {
				if (spawnDetail.SpawnPointName == SpawnPointName)
					spawnHistory.Add(spawnDetail);
			}

			
			if (!spawnHistory.IsEmpty())
			{				
				int32 startIndex = spawnHistory.Num() - 1 - condition.BeginningFromHistoryIndex;
				int32 lastIndex = startIndex - condition.NumberOfSpawnsToCheck + 1;
				for (int32 Index = startIndex; Index >= 0 && Index >= lastIndex; --Index)
				{
					FSpawnDetails spawn = spawnHistory[Index];					
					FRegexMatcher matcher(regexPattern, spawn.EnemyConfig);
					if (matcher.FindNext())
						++numberOfMatches;
					
				}
			}
			
			isApplicable = isApplicable && condition.MinNumberOfMatches <= numberOfMatches;
		}
		if (isApplicable)
			applicableRemarks.Add(remark);
	}

	return applicableRemarks;
}
