#pragma once

#include "CoreMinimal.h"
#include "Participation.generated.h"

USTRUCT(BlueprintType)
struct FParticipation {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) FString id;
	UPROPERTY(BlueprintReadOnly) bool submittedSurvey;
	UPROPERTY(BlueprintReadOnly) FString gameConfigId;
	UPROPERTY(BlueprintReadOnly) bool assignGameConfig;
};

