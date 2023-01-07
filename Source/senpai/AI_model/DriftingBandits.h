// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AImodel.h"
#include "DriftingBandits.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SENPAI_API UDriftingBandits : public UAImodel
{
	GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDriftingBandits();

    UFUNCTION(BlueprintCallable)
    void init(TArray<FString> enemies);

    //create/set all possible enemies in Qtable with 0

    void initQtable();

    FString createEnemy();

    void giveReward(const FString enemyIn, double reward);

    FString printQtable() const;

    TMap<FString, double> getQtable() const;

    void setQtable(const TMap<FString, double> Qtable);

    TArray<double> getQArray() const;

    void setQArray(const TArray<double> Qarray);

    TArray<int> getNArray() const;

    void setNArray(const TArray<int> Narray);

    TArray<int> getTArray() const;

    void setTArray(const TArray<int> Tarray);

    TArray<FString> getIndex2Name() const;

    void setIndex2Name(const TArray<FString> i2n);


protected:
    TMap<FString, unsigned short> m_Name2Index;
    TArray<FString> m_Index2Name;
    TArray<double> m_Qtable;
    TArray<int> m_Ntable;
    TArray<int> m_Ttable;
    unsigned short m_EnemyCount;
    double m_rho;
    double m_beta;

    
    void addEnemy(FString enemy);
    void populateEnemies();
	
};
