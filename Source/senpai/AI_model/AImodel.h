// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <assert.h>  
#include "Math/UnrealMathUtility.h"
#include "CoreMinimal.h"
#include "Core.h"
#include "Components/ActorComponent.h"
#include "AImodel.generated.h"


UCLASS(Abstract, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENPAI_API UAImodel : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAImodel();

    //create/set all possible enemies in Qtable with 0
    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual void initQtable() PURE_VIRTUAL(UAImodel::initQtable, );

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual FString createEnemy() PURE_VIRTUAL(UAImodel::createEnemy, return "";);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual void giveReward(const FString enemyIn, double reward) PURE_VIRTUAL(UAImodel::createEnemy, );

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual TMap<FString, double> getQtable() const PURE_VIRTUAL(UAImodel::createEnemy, return TMap<FString, double>{};);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual void setQtable(const TMap<FString, double> Qtable) PURE_VIRTUAL(UAImodel::createEnemy,);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual TArray<double> getQArray() const PURE_VIRTUAL(UAImodel::createEnemy, return TArray<double>{};);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual void setQArray(const TArray<double> Qarray) PURE_VIRTUAL(UAImodel::createEnemy,);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual TArray<int> getNArray() const PURE_VIRTUAL(UAImodel::createEnemy, return TArray<int>{};);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual void setNArray(const TArray<int> Narray) PURE_VIRTUAL(UAImodel::createEnemy, );

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual TArray<int> getTArray() const PURE_VIRTUAL(UAImodel::createEnemy, return TArray<int>{};);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual void setTArray(const TArray<int> Tarray) PURE_VIRTUAL(UAImodel::createEnemy, );

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual TArray<FString> getIndex2Name() const PURE_VIRTUAL(UAImodel::createEnemy, return TArray<FString>{};);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual void setIndex2Name(const TArray<FString> i2n) PURE_VIRTUAL(UAImodel::createEnemy, );

    UFUNCTION(BlueprintCallable, Category = "AImodel")
        virtual FString printQtable() const PURE_VIRTUAL(UAImodel::createEnemy, return "IM VIRTUAL";);


    UFUNCTION(BlueprintCallable, Category = "AImodel")
        TArray<int> enemyToIntArray(const FString enemy) const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    // helper functions
    TArray<double> softmax(const TArray<double> Qvalues, const double beta) const;

    TArray<double> cumsum(const TArray<double> pvalues) const;

    int rollFromProb(const TArray<double> pvalues) const;

    unsigned short m_TagCount;
    
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

USTRUCT()
struct FNestedStringArray {
    GENERATED_BODY()
        //UPROPERTY(EditAnywhere)
        TArray<FString> A;
};

USTRUCT()
struct FNestedIntArray {
    GENERATED_BODY()
        //UPROPERTY(EditAnywhere)
        TArray<int> A;
};
