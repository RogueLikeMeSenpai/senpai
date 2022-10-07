// Fill out your copyright notice in the Description page of Project Settings.


#include "AImodel.h"

// Sets default values for this component's properties
UAImodel::UAImodel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAImodel::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




// Called every frame
void UAImodel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// helper functions
TArray<double> UAImodel::softmax(const TArray<double> Qvalues, const double beta) const {
    // from https://slaystudy.com/implementation-of-softmax-activation-function-in-c-c/
    TArray<double> softmax;
    double m, sum, constant;

    m = -INFINITY;
    for (double Qi : Qvalues) {
        if (m < Qi) {
            m = Qi;
        }
    }

    sum = 0.0;
    for (double Qi : Qvalues) {
        sum += exp(beta * (Qi - m));
    }

    constant = beta * m + log(sum);
    for (double Qi : Qvalues) {
        softmax.Add(exp(beta * Qi - constant));
    }

    return softmax;
}

TArray<double> UAImodel::cumsum(const TArray<double> pvalues) const {
    TArray<double> cumsum;
    double psum = 0.;
    for (double p : pvalues) {
        psum += p;
        cumsum.Add(psum);
    }
    return cumsum;
}

int UAImodel::rollFromProb(const TArray<double> pvalues) const {
    int i = 0;
    TArray<double> cumpvalues = cumsum(pvalues);
    double roll = (double)FMath::RandRange(0., 1.);
    for (double cump : cumpvalues) {
        if (roll > cump) ++i;
    }
    if (i == pvalues.Num()) --i; // for rare case, that RNG rolls 1.0 and softmax has a rounding error
    return i;
}

TArray<int> UAImodel::enemyToIntArray(const FString enemy) const {
    int enemyInt = FCString::Atoi(*enemy);;
    TArray<int> enemyArr;
    enemyArr.Init(0, m_TagCount);

    for (int i = m_TagCount - 1; i >= 0;--i) {
        enemyArr[i] = enemyInt % 10; // get last digit
        enemyInt /= 10;
    }

    return enemyArr;

}

