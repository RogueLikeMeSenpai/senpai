// Fill out your copyright notice in the Description page of Project Settings.


#include "SenpaiGameInstanceBase.h"

USenpaiGameInstanceBase::USenpaiGameInstanceBase()
{
	gameDataTracker = CreateDefaultSubobject<UGameDataTracker>(TEXT("GameDataTracker0"));
	m_projectVersion = GetProjectVersion();
}

void USenpaiGameInstanceBase::RegisterLevelChange(int32 Level, bool death)
{
	if(this->CurrentLevel != Level)
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

void USenpaiGameInstanceBase::track(FString name, TMap<FString, FString> data)
{
	FTrackingEvent event;
	event.projectVersion = m_projectVersion;
	event.data = data;
	event.name = name;
	event.level = this->CurrentLevel;
	event.run = this->CurrentRun;
	event.timestamp = FDateTime::UtcNow();
	event.participationId = this->gameDataTracker->participation.id;
	event.gameConfigId = this->gameDataTracker->participation.gameConfigId;

	this->gameDataTracker->track(event);
}

FString USenpaiGameInstanceBase::GetProjectVersion()
{
	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}
