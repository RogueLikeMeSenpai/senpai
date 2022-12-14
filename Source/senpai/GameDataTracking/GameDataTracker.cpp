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
    TArray<TSharedPtr<FJsonValue>> eventsJsonArray;
    for (auto& trackingEvent : events)
    {        
        TSharedPtr<FJsonObject> eventJsonObject = MakeShareable(new FJsonObject);        
        eventJsonObject->SetStringField("name", trackingEvent.name);
        eventJsonObject->SetStringField("gameConfiguration", trackingEvent.gameConfiguration);
        eventJsonObject->SetNumberField("run", trackingEvent.run);
        eventJsonObject->SetNumberField("level", trackingEvent.level);
        eventJsonObject->SetStringField("participant", trackingEvent.participant);
        eventJsonObject->SetNumberField("timestamp", trackingEvent.timestamp.ToUnixTimestamp());

        TSharedPtr<FJsonObject> dataJsonObject = MakeShareable(new FJsonObject);
        for (auto& dataPair : trackingEvent.data)
        {
            dataJsonObject->SetStringField(dataPair.Key, dataPair.Value);
        }
        eventJsonObject->SetObjectField("data", dataJsonObject);
        
        eventsJsonArray.Add(MakeShareable(new FJsonValueObject(eventJsonObject)));
    }

    FString jsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonString);
    FJsonSerializer::Serialize(eventsJsonArray, Writer);
    return jsonString;  
}
