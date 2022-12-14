// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataTracker.h"
#include "Json.h"

void UGameDataTracker::track(FTrackingEvent event)
{
	UE_LOG(LogTemp, Display, TEXT("Tracked event: %s"), *event.name);
	events.Add(event);
}

FString UGameDataTracker::toJson()
{
    TArray<TSharedPtr<FJsonValue>> jsonArray;
    for (auto& trackingEvent : events)
    {        
        TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject);        
        jsonObject->SetStringField("name", trackingEvent.name);
        jsonObject->SetNumberField("run", trackingEvent.run);
        
        jsonArray.Add(MakeShareable(new FJsonValueObject(jsonObject)));
    }

    FString jsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonString);
    FJsonSerializer::Serialize(jsonArray, Writer);
    return jsonString;  
}
