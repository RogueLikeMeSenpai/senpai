// Fill out your copyright notice in the Description page of Project Settings.


#include "SenpaiGameInstanceBase.h"

void USenpaiGameInstanceBase::RegisterLevelChange(int32 Level, bool death)
{
	this->checkpoint = "";
	this->CurrentLevel = Level;
	if (this->CurrentLevel == 0 || death)
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
