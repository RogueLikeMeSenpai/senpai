// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Participation.h"
#include "ParticipationSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SENPAI_API UParticipationSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FParticipation participation;
	
};
