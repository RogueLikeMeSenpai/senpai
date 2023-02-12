// Fill out your copyright notice in the Description page of Project Settings.


#include "GameDataTracker.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "ParticipationSaveGame.h"
#include "Kismet/GameplayStatics.h"


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
        eventJsonObject->SetStringField("gameConfigId", trackingEvent.gameConfigId);
        eventJsonObject->SetNumberField("run", trackingEvent.run);
        eventJsonObject->SetNumberField("level", trackingEvent.level);
        eventJsonObject->SetStringField("participationId", trackingEvent.participationId);
        eventJsonObject->SetStringField("timestamp", trackingEvent.timestamp.ToIso8601());

        if (!trackingEvent.data.IsEmpty()) {
            TSharedPtr<FJsonObject> dataJsonObject = MakeShareable(new FJsonObject);
            for (auto& dataPair : trackingEvent.data)
            {
                dataJsonObject->SetStringField(dataPair.Key, dataPair.Value);
            }
            eventJsonObject->SetObjectField("data", dataJsonObject);
        }
        
        eventsJsonArray.Add(MakeShareable(new FJsonValueObject(eventJsonObject)));
    }

    FString jsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonString);
    FJsonSerializer::Serialize(eventsJsonArray, Writer);
    return jsonString;  
}

void UGameDataTracker::writeToFile(FString content, FString fileName)
{
    //FString filePath = FPaths::ProjectUserDir();
    //filePath.Append(fileName);
    FString filePath = FPaths::Combine(participationDirectory, fileName);
    
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

    TSharedRef Request = PostRequest("", ContentJsonString, TEXT("application/json"));
    Request->OnProcessRequestComplete().BindUObject(this, &UGameDataTracker::httpResponse);
    Send(Request);
}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UGameDataTracker::PostRequest(FString Subroute, FString Content, FString contentType)
{
    UE_LOG(LogTemp, Display, TEXT("post request on subroute %s with content %s and type %s"), *Subroute, *Content, *contentType);
    TSharedRef Request = RequestWithRoute(Subroute, contentType);
    Request->SetVerb("POST");
    Request->SetContentAsString(Content);
    return Request;
}

//TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UGameDataTracker::GetRequest(FString Subroute)
//{
//    TSharedRef Request = RequestWithRoute(Subroute, "");
//    Request->SetVerb("GET");
//    return TSharedRef<IHttpRequest, ESPMode::ThreadSafe>();
//}

TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UGameDataTracker::RequestWithRoute(FString Subroute, FString contentType)
{
    //<IHttpRequest, ESPMode::ThreadSafe>
    TSharedRef Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(ApiBaseUrl + Subroute);
    SetRequestHeaders(Request, contentType);
    return Request;
}

void UGameDataTracker::SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request, FString contentType)
{
    Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
    Request->SetHeader(TEXT("Content-Type"), contentType);
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
        UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code %d and response %s"), Response->GetResponseCode(), *Response->GetContentAsString());
        
        return false; 
    }
    return false;
}


void UGameDataTracker::requestAuthToken(FString username, FString password, FDelegateLoggedIn delegateLoggedIn)
{    
    this->m_delegateLoggedIn = delegateLoggedIn;    

    //FText contentText = FText::FormatOrdered(FTextFormat::FromString("grant_type=password&username={0}&password={1}"), username, password);
    //FString content = contentText.ToString();    
    FString content = "grant_type=password&username=";
    content.Append(username);
    content.Append("&password=");
    content.Append(password);

    TSharedRef Request = PostRequest(this->tokenEndpoint, content, TEXT("application/x-www-form-urlencoded"));
    Request->OnProcessRequestComplete().BindUObject(this, &UGameDataTracker::authTokenResponse);
    Send(Request);

    this->user = EmptyUser;
    this->user.email = username;
}

//void UGameDataTracker::TestMyDelegate(FDelegateTest myDelegate)
//{
//    this->m_delegate = myDelegate;
//    // this->requestAuthToken("max.deutsch@posteo.at", "test");
//}

bool UGameDataTracker::isLoggedIn()
{
    if (this->user.loggedIn) {
        // FTimesthis->user->loggedInTimestamp + FTimespan(0, 0, this->user->authToken.expires_in);
        return FDateTime::UtcNow() < this->user.tokenExpirationTimestampUtc;
    }
    return false;
}

void UGameDataTracker::fetchParticipation(FString participationId, bool completeGameplay, bool completeDataUpload)
{
    // this->participation.id = participationId;
    // this->participation.gameConfigurationId = "fetched gc";
    // this->onParticipationChange.Broadcast(participation, true);

    FParticipation participationLogin;
    participationLogin.id = participationId;
    participationLogin.assignGameConfig = true;
    participationLogin.completeGameplay = completeGameplay;
    participationLogin.completeDataUpload = completeDataUpload;

    FString ContentJsonString;
    GetJsonStringFromStruct(participationLogin, ContentJsonString);

    TSharedRef Request = PostRequest(loginEndpoint, ContentJsonString, TEXT("application/json"));
    Request->OnProcessRequestComplete().BindUObject(this, &UGameDataTracker::fetchParticipationResponse);
    Send(Request);
    return;
}

void UGameDataTracker::loadParticipation()
{
    if (UGameplayStatics::DoesSaveGameExist(participationSlotName, 0)) 
    {
        UParticipationSaveGame* saveGame = Cast<UParticipationSaveGame>(UGameplayStatics::LoadGameFromSlot(participationSlotName, 0));
        if (saveGame) 
        {
            this->participation = saveGame->participation;
            this->onParticipationChange.Broadcast(this->participation, true, "");
        }
    }    
    else
    {
        this->onParticipationChange.Broadcast(this->participation, false, "");
    }
    
}

void UGameDataTracker::saveParticipation()
{
    UParticipationSaveGame* saveGame = Cast<UParticipationSaveGame>(UGameplayStatics::CreateSaveGameObject(UParticipationSaveGame::StaticClass()));
    saveGame->participation = this->participation;
    UGameplayStatics::SaveGameToSlot(saveGame, participationSlotName, 0);
}

void UGameDataTracker::deleteParticipation()
{
    UGameplayStatics::DeleteGameInSlot(participationSlotName, 0);
    this->onParticipationChange.Broadcast(this->participation, false, "");
}

void UGameDataTracker::authTokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!ResponseIsValid(Response, bWasSuccessful))
        return;

    UE_LOG(LogTemp, Display, TEXT("httpResponse: Success: %s"), *FString((bWasSuccessful ? "true" : "false")));
    FString responseContent = Response->GetContentAsString();
    UE_LOG(LogTemp, Display, TEXT("httpResponse: content: %s"), *responseContent);

    FAuthToken responseStruct;
    GetStructFromJsonString(Response, responseStruct);
    //UE_LOG(LogTemp, Warning, TEXT("httpResponse: accessToken: %s"), *responseStruct.access_token);
    
    
    this->user.loggedIn = true;
    this->user.authToken = responseStruct;
    this->user.loggedInTimestampUtc = FDateTime::UtcNow();
    this->user.tokenExpirationTimestampUtc = this->user.loggedInTimestampUtc + FTimespan(0, 0, responseStruct.expires_in);

    this->m_delegateLoggedIn.ExecuteIfBound(this->user);
    // this->m_delegate.ExecuteIfBound();

}

void UGameDataTracker::fetchParticipationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!ResponseIsValid(Response, bWasSuccessful)) {
        this->onParticipationChange.Broadcast(this->participation, false, Response->GetContentAsString());
        return;
    }
    GetStructFromJsonString(Response, this->participation);
    this->onParticipationChange.Broadcast(this->participation, true, "");
    this->saveParticipation();
}

void UGameDataTracker::startPersistEventsTimer(float rate)
{
    GetWorld()->GetTimerManager().SetTimer(PersistEventsTimerHandle, this, &UGameDataTracker::PersistEvents, rate, true);
}

void UGameDataTracker::PersistEvents()
{
    UE_LOG(LogTemp, Display, TEXT("OnPersistEvents started"));
    if (events.IsEmpty()) {
        UE_LOG(LogTemp, Display, TEXT("OnPersistEvents finished. No events to persist"));
        return;
    }
    
    FString json = toJson();
    events.Empty(); // TODO thread safe?
    FString fileName = FPaths::CreateTempFilename(TEXT(""), TEXT("ge_"), TEXT(""));
    //FString fileName = FString::Printf(TEXT("ge_%lld"), FDateTime::UtcNow().ToUnixTimestamp());
    writeToFile(json, fileName);
    UE_LOG(LogTemp, Display, TEXT("OnPersistEvents finished"));
}

void UGameDataTracker::UploadEvents()
{

    // FFileHelper::
    TArray<FString> files;
    FString pathWildcard = FPaths::Combine(participationDirectory, "ge_*");
    IFileManager& FileManager = IFileManager::Get();
    FileManager.FindFiles(files, *pathWildcard, true, false);

    for (auto& fileName : files) {
        FString fullFilePath = FPaths::Combine(participationDirectory, fileName);
        FString content;
        if (FFileHelper::LoadFileToString(content, *fullFilePath, FFileHelper::EHashOptions::None)) {
            UE_LOG(LogTemp, Display, TEXT("Read file '%s'"), *fullFilePath);
            track(content);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Could not read file '%s'"), *fullFilePath, *content);
        }        
    }
    fetchParticipation(participation.id, false, true);
}

void UGameDataTracker::track(FString& ContentJsonString)
{
    TSharedRef Request = PostRequest(trackEndpoint, ContentJsonString, TEXT("application/json"));
    Request->OnProcessRequestComplete().BindUObject(this, &UGameDataTracker::trackResponse);
    Send(Request);
    return;

}

void UGameDataTracker::trackResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!ResponseIsValid(Response, bWasSuccessful)) {

        return;
    }
    FString responseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Display, TEXT("successfully uploaded event data. response: '%s'"), *responseString);
}
