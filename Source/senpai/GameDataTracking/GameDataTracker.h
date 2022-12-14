// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameDataTracker.generated.h"


USTRUCT(BlueprintType)
struct FTrackingEvent {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString gameConfiguration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 run;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString participant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime timestamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FString> data;
};

/**
 * 
 */
UCLASS()
class SENPAI_API UGameDataTracker : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void track(FTrackingEvent event);

	UFUNCTION(BlueprintCallable)
	FString toJson();
	
private:
	UPROPERTY()
	TArray<FTrackingEvent> events;
};
