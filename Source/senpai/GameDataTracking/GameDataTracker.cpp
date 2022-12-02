// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataTracker.h"

void UGameDataTracker::track(FTrackingEvent event)
{
	UE_LOG(LogTemp, Display, TEXT("Tracked event: %s"), *event.name);
	events.Add(event);
}
