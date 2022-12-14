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
    // Json object
    //TSharedPtr<FJsonObject> MyJson = MakeShareable(new FJsonObject);
    //MyJson->SetStringField("MyStringFieldKey", FString("MyStringfieldValue"));
    //MyJson->SetNumberField("MyNumberFieldKey", 42);

    //FString MyJsonFString;
    //TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&MyJsonFString);
    //FJsonSerializer::Serialize(MyJson.ToSharedRef(), Writer);

    // Json array
    TArray<TSharedPtr<FJsonValue>> MyJsonArray;
    for (auto& Element : events)
    {
        MyJsonArray.Add(MakeShareable(new FJsonValueString(Element.name)));
    }
    FString MyJsonFString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&MyJsonFString);
    FJsonSerializer::Serialize(MyJsonArray, Writer);
    return MyJsonFString;


    //TArray<TSharedPtr<FJsonValue>> MyJsonArray;
    //// Put some data in the JSON array

    //for (auto & Element : MyFNameArray)
    //{

    //    MyJsonArray.Add(MakeShareable(new FJsonValueString(Element.ToString())));

    //}
    //FString MyJsonFString;
    //TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&MyJsonFString);
    //FJsonSerializer::Serialize(MyJsonArray, Writer);
    //return MyJsonFString;
    
}
