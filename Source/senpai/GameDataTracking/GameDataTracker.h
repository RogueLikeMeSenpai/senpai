// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "Templates/SharedPointer.h"
#include "Interfaces/IHttpRequest.h"
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
	TMap<FString, FString> data; // TODO change to array?
};

USTRUCT()
struct FAWSHelloWorld_Request
{
	GENERATED_BODY()
	UPROPERTY() FString firstName;
	UPROPERTY() FString lastName;
};

USTRUCT()
struct FAWSHelloWorld_Response
{
	GENERATED_BODY()
	UPROPERTY() FString body;
};


USTRUCT()
struct FAuthToken
{
	GENERATED_BODY()
	UPROPERTY() FString access_token;
	UPROPERTY() FString token_type;
	UPROPERTY() int32 expires_in;
	UPROPERTY() FString refresh_token; // TODO persist?
};

USTRUCT()
struct FAuthTokenResponse {
	GENERATED_BODY()
	UPROPERTY() FAuthToken body;
};

//typedef TSharedRef<IHttpRequest, ESPMode::ThreadSafe> TSharedRefHttpRequest;

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

	UFUNCTION(BlueprintCallable)
	void writeToFile(FString content, FString fileName);

	UFUNCTION(BlueprintCallable)
	void makeHttpRequest();
	
	UFUNCTION(BlueprintCallable)
	void requestAuthToken(FString username, FString password);
	
	
private:
	UPROPERTY()
	TArray<FTrackingEvent> events;

	FAuthToken authToken;

	FString ApiBaseUrl = "https://dreamy-kelpie-61e7a3.netlify.app";
	FString tokenEndpoint = "/.netlify/identity/token";

	IHttpRequest* test;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PostRequest(FString Subroute, FString Content, FString contentType);
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> RequestWithRoute(FString Subroute, FString contentType);
	void SetRequestHeaders(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request, FString contentType);
	void httpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void Send(TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

	template<typename StructType>
	void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);

	template<typename StructType>
	void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);
	
	void authTokenResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
