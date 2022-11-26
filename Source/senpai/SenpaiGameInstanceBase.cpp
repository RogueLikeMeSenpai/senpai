// Fill out your copyright notice in the Description page of Project Settings.


#include "SenpaiGameInstanceBase.h"

void USenpaiGameInstanceBase::ChangeLevel(int32 Level)
{
	this->CurrentLevel = Level;
	if (this->CurrentLevel == 0)
		++this->CurrentRun;
}

void USenpaiGameInstanceBase::RegisterSpawn(FString SpawnPointName, FString EnemyConfig)
{
	FSpawnDetails spawnDetails;
	spawnDetails.Run = this->CurrentRun;
	spawnDetails.Level = this->CurrentLevel;
	spawnDetails.SpawnPointName = SpawnPointName;
	spawnDetails.EnemyConfig = EnemyConfig;

	this->SpawnHistory.Add(spawnDetails);	
}
