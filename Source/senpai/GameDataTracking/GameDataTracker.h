// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
// #include "Templates/SharedPointer.h"
#include "Interfaces/IHttpRequest.h"
#include "Participation.h"
#include "GameDataTracker.generated.h"


USTRUCT(BlueprintType)
struct FTrackingEvent {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString projectVersion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString gameConfigId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 run;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString participationId;
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
	UPROPERTY() int32 expires_in; // seconds
	UPROPERTY() FString refresh_token; // TODO persist?
};


USTRUCT()
struct FUserResponse {
	GENERATED_BODY()
	UPROPERTY() FString id;
	UPROPERTY() FString email;
	UPROPERTY() FString confirmation_sent_at;
	UPROPERTY() FString created_at;
	UPROPERTY() FString updated_at;
};

USTRUCT(BlueprintType)
struct FAuthUser {
	GENERATED_BODY()
	UPROPERTY() bool loggedIn;
	UPROPERTY(BlueprintReadOnly) FString email;
	UPROPERTY() FAuthToken authToken;
	UPROPERTY() FDateTime loggedInTimestampUtc;
	UPROPERTY(BlueprintReadOnly) FDateTime tokenExpirationTimestampUtc;
	
};


//typedef TSharedRef<IHttpRequest, ESPMode::ThreadSafe> TSharedRefHttpRequest;

static const struct FAuthUser EmptyUser = {};
static const FString participationSlotName = "participation";

//DECLARE_DYNAMIC_DELEGATE(FDelegateTest);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDelegateLoggedIn, FAuthUser, user);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnParticipationChange, FParticipation, participation, bool, valid, FString, error);

/**
 * 
 */
UCLASS()
class SENPAI_API UGameDataTracker : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FString websiteUrl = "https://sanu.at/";

	UFUNCTION(BlueprintCallable)
	void track(FTrackingEvent event);

	UFUNCTION(BlueprintCallable)
	FString toJson();

	UFUNCTION(BlueprintCallable)
	void writeToFile(FString content, FString fileName);

	UFUNCTION(BlueprintCallable)
	void makeHttpRequest();
	
	UFUNCTION(BlueprintCallable)
	void requestAuthToken(FString username, FString password, FDelegateLoggedIn delegateLoggedIn);

	//UFUNCTION(BlueprintCallable)
	//void TestMyDelegate(FDelegateTest myDelegate);

	UFUNCTION(BlueprintCallable)
	bool isLoggedIn();

	UPROPERTY(BlueprintReadOnly) FParticipation participation;
	UPROPERTY(BlueprintAssignable) FOnParticipationChange onParticipationChange;

	UFUNCTION(BlueprintCallable) 
	void fetchParticipation(FString participationId, bool completeGameplay, bool completeDataUpload);

	UFUNCTION(BlueprintCallable)
	void loadParticipation();

	UFUNCTION(BlueprintCallable)
	void saveParticipation();

	UFUNCTION(BlueprintCallable)
	void deleteParticipation();
	
	
private:
	//UPROPERTY()
	//FDelegateTest m_delegate;

	UPROPERTY()
	FDelegateLoggedIn m_delegateLoggedIn;

	UPROPERTY()
	TArray<FTrackingEvent> events;

	UPROPERTY()
	FAuthUser user;	

	FTimerHandle PersistEventsTimerHandle;


	 const FString ApiBaseUrl = "https://sanu.at";
	//const FString ApiBaseUrl = "http://localhost:8888";
	const FString tokenEndpoint = "/.netlify/identity/token";
	const FString loginEndpoint = "/.netlify/functions/login-participation";
	const FString trackEndpoint = "/.netlify/functions/track";

	const FString participationDirectory = FPaths::Combine(FPaths::ProjectUserDir(), "participation");

	IHttpRequest* test;

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> PostRequest(FString Subroute, FString Content, FString contentType);
	//TSharedRef<IHttpRequest, ESPMode::ThreadSafe> GetRequest(FString Subroute);

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
	void fetchParticipationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void track(FString& content);
	void trackResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/*void requestUser();
	void userResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);*/

	UFUNCTION(BlueprintCallable)
	void startPersistEventsTimer(float rate);

	UFUNCTION(BlueprintCallable)
	void PersistEvents();

	UFUNCTION(BlueprintCallable)
	void UploadEvents();

};
