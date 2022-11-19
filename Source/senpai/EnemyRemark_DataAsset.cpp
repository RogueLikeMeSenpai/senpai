// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRemark_DataAsset.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>



UEnemyRemark_DataAsset::UEnemyRemark_DataAsset()
{
	

	//YourGameInstanceClass* GI = Cast<YourGameInstanceClass>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GI)
	//{
	//	// Do Something
	//}
}

TArray<FEnemyRemark> UEnemyRemark_DataAsset::FindApplicableRemarks(USenpaiGameInstanceBase* gameInstance, FString SpawnPointName, FString EnemyConfig)
{
	
	//USenpaiGameInstanceBase* gameInstance = 
	//UWorld* w = GetWorld();
	//UGameInstance* gi = UGameplayStatics::GetGameInstance(w);
	//USenpaiGameInstanceBase* gameInstance = CastChecked<USenpaiGameInstanceBase>(gi);
	//USenpaiGameInstanceBase* gameInstance = w->GetGameInstanceChecked<USenpaiGameInstanceBase>();
	/*UGameInstance* gi = UGameplayStatics::GetGameInstance(GetWorld());
	USenpaiGameInstanceBase* gameInstance = Cast<USenpaiGameInstanceBase>(gi);*/
	if (gameInstance == nullptr)
		return TArray<FEnemyRemark>();

	auto spawnHistory = gameInstance->SpawnHistory;
	TArray<FEnemyRemark> applicableRemarks;
	for (const auto& remark: remarks)
	{
		bool isApplicable = true;
		for (const auto& condition : remark.Conditions)
		{
			const FRegexPattern regexPattern(condition.MatchEnemyConfigRegex);			
			int32 numberOfMatches = 0;
			
			if (!spawnHistory.IsEmpty())
			{
				// Traversing backwards because the first
				int32 lastIndex = spawnHistory.Num() - condition.NumberOfSpawnsToCheck;
				for (int32 Index = spawnHistory.Num() - 1; Index >= 0 && Index >= lastIndex; --Index)
				//for (int32 Index = condition.BeginningFromHistoryIndex; Index < spawnHistory.Num() && Index < condition.NumberOfSpawnsToCheck; ++Index)
				{
					FRegexMatcher matcher(regexPattern, spawnHistory[Index].EnemyConfig);
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
