// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataTracker.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"
#include "Runtime/Online/HTTP/Public/Http.h"

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

void UGameDataTracker::writeToFile(FString content, FString fileName)
{
    FString filePath = FPaths::ProjectUserDir();
    filePath.Append(fileName);

    IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
    
    if (FFileHelper::SaveStringToFile(content, *filePath))
    {
        UE_LOG(LogTemp, Display, TEXT("FileManipulation: Successfully Written '%s'"), *filePath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write content to '%s'"), *filePath);
    }

    //if (FileManager.FileExists(*filePath))
    //{
    //    // FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
    //    //if (FFileHelper::SaveStringToFile(content, *filePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append))
    //    if (FFileHelper::SaveStringToFile(content, *filePath))
    //    {
    //        UE_LOG(LogTemp, Display, TEXT("FileManipulation: Successfully Written '%s'"), *filePath);
    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write content to '%s'"), *filePath);
    //    }
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("FileManipulation: File not existing '%s'"), *filePath);
    //}








    ////We will use this FileManager to deal with the file.
    //IPlatformFile & FileManager = FPlatformFileManager::Get().GetPlatformFile();

    //FString StringToWrite(TEXT("Hello World. Written from Unreal Engine 4"));
    //// Always first check if the file that you want to manipulate exist.
    //if (FileManager.FileExists(*file))
    //{
    //    // We use the LoadFileToString to load the file into
    //    if (FFileHelper::SaveStringToFile(StringToWrite, *file))
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Sucsesfuly Written: \"%s\" to the text file"), *StringToWrite);
    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Failed to write FString to file."));
    //    }
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("FileManipulation: ERROR: Can not read the file because it was not found."));
    //    UE_LOG(LogTemp, Warning, TEXT("FileManipulation: Expected file location: %s"), *file);
    //}
}

// TODO response

template<typename StructType>
inline void UGameDataTracker::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput)
{
    FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);
}

template<typename StructType>
void UGameDataTracker::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput)
{
    StructType StructData;
    FString JsonString = Response->GetContentAsString();
    FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &StructOutput, 0, 0);
}

void UGameDataTracker::makeHttpRequest()
{
    
    FAWSHelloWorld_Request structRequest;
    structRequest.firstName = "Unreal";
    structRequest.lastName = "Engine";

    FString ContentJsonString;
    GetJsonStringFromStruct(structRequest, ContentJsonString);

    TSharedRef Request = PostRequest("", ContentJsonString);
    Request->OnProcessRequestComplete().BindUObject(this, &UGameDataTracker::httpResponse);
    Send(Request);
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UGameDataTracker::PostRequest(FString Subroute, FString ContentJsonString)
{
    TSharedRef Request = RequestWithRoute(Subroute);
    Request->SetVerb("POST");
    Request->SetContentAsString(ContentJsonString);
    return Request;
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UGameDataTracker::RequestWithRoute(FString Subroute)
{
    //<IHttpRequest, ESPMode::ThreadSafe>
    TSharedRef Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(ApiBaseUrl + Subroute);
    SetRequestHeaders(Request);
    return Request;
}

void UGameDataTracker::SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request)
{
    Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

void UGameDataTracker::httpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!ResponseIsValid(Response, bWasSuccessful)) 
        return;

    UE_LOG(LogTemp, Display, TEXT("httpResponse: Success: %s"), *FString((bWasSuccessful ? "true" : "false")));
    FString responseContent = Response->GetContentAsString();
    UE_LOG(LogTemp, Display, TEXT("httpResponse: content: %s"), *responseContent);

    FAWSHelloWorld_Response responseStruct;
    GetStructFromJsonString(Response, responseStruct);
    UE_LOG(LogTemp, Warning, TEXT("httpResponse: body: %s"), *responseStruct.body);

    /*FResponse_Login LoginResponse;
    GetStructFromJsonString(Response, LoginResponse);*/
}

void UGameDataTracker::Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request)
{

    Request->ProcessRequest();
}

bool UGameDataTracker::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid()) 
        return false;

    if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
        return true; 
    else 
    { 
        UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode()); 
        return false; 
    }
    return false;
}
