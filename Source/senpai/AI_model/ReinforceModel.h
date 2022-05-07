// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <assert.h>  
#include "Math/UnrealMathUtility.h"
#include "CoreMinimal.h"
#include "Core.h"
#include "Components/ActorComponent.h"
#include "ReinforceModel.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENPAI_API UReinforceModel : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReinforceModel();
   
    UFUNCTION(BlueprintCallable, Category = "AImodel")
    FString createEnemy();

    UFUNCTION(BlueprintCallable, Category = "AImodel")
    TArray<int> enemyToIntArray(const FString enemy) const;

    UFUNCTION(BlueprintCallable, Category = "AImodel")
    void giveReward(const FString enemyIn, double reward);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
    TMap<FString, double> getQtable() const;

    UFUNCTION(BlueprintCallable, Category = "AImodel")
    void setQtable(const TMap<FString, double> Qtable);

    UFUNCTION(BlueprintCallable, Category = "AImodel")
    FString printQtable() const;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    TMap<FString, double> m_Qtable;
    TArray<FNestedIntArray> m_Layers;
    unsigned short m_LayerCount;
    unsigned short m_TagCount;
    FString blankEnemy;
    

    void add2Layer(unsigned short iLayer, unsigned int tagNumber);

    void add2Layer(unsigned short iLayer, TArray<unsigned int> tagNumbers);

    //create all possible enemies in Qtable
    void initQtable();

    // helper functions
    TArray<double> softmax(const TArray<double> Qvalues) const;

    TArray<double> cumsum(const TArray<double> pvalues) const;

    int rollFromProb(const TArray<double> pvalues) const;

    bool QtableContains(FString enemy);

    bool checkQtableValid(const TMap<FString, double> Qtable) const;


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