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
    //new FJsonObject();
    TSharedPtr<FJsonObject> MyJson = MakeShareable(new FJsonObject);
    MyJson->SetStringField("MyStringFieldKey", FString("MyStringfieldValue"));
    MyJson->SetNumberField("MyNumberFieldKey", 42);

    FString MyJsonFString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&MyJsonFString);
    FJsonSerializer::Serialize(MyJson.ToSharedRef(), Writer);

    return MyJsonFString;
    //TSharedPtr<FJsonObject> MyJson = MakeShareable(new FJsonObject());

    //MyJson->SetStringField("MyStringFieldKey", FString("MyStringFieldValue"));

    //MyJson->SetNumberField("MyNumberFieldKey", 42);



    //FString MyJsonFString;

    //TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&MyJsonFString);

    //FJsonSerializer::Serialize(MyJson.ToSharedRef(), Writer);
}
