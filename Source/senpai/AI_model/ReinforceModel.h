// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AImodel.h"
#include "ReinforceModel.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SENPAI_API UReinforceModel : public UAImodel
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReinforceModel();


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
	// Called when the game starts
	virtual void BeginPlay() override;

    
    TMap<FString, double> m_Qtable;
    TArray<FNestedIntArray> m_Layers;
    unsigned short m_LayerCount;
    unsigned short m_TagCount;
    float m_maxQvalue;
    float m_minQvalue;
    FString blankEnemy;
    

    void add2Layer(unsigned short iLayer, unsigned int tagNumber);

    void add2Layer(unsigned short iLayer, TArray<unsigned int> tagNumbers);

    bool QtableContains(FString enemy);

    bool checkQtableValid(const TMap<FString, double> Qtable) const;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};

