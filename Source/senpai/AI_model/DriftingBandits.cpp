// Fill out your copyright notice in the Description page of Project Settings.


#include "DriftingBandits.h"

UDriftingBandits::UDriftingBandits() : m_Name2Index({}) {
}

void UDriftingBandits::init(TArray<FString> enemies)
{
    m_EnemyCount = 0;
    m_rho = 0.7;
    m_beta = 1.0;
    m_TagCount = 8;
    if (enemies.IsEmpty())
    {
        populateEnemies();
    }
    else
    {
        for (FString enemy : enemies)
        {
            addEnemy(enemy);
        }
    }
}

void UDriftingBandits::addEnemy(FString enemy) {
    for (FString e : m_Index2Name) { //check if enemy does already exist
        if (e == enemy) return;
    }
    m_Index2Name.Add(enemy);
    m_Qtable.Add(0.);
    m_Ntable.Add(0);
    m_Ttable.Add(0);
    m_Name2Index.Add(enemy, m_EnemyCount);
    m_EnemyCount++;
  
}

/*
populate according to current plan:
only 1 weapon from tags [0,2]
only 1 behavior from tags [3,4]
only 1 armor from tags [5,7]
*/
void UDriftingBandits::populateEnemies() {
    FString blankenemy = "00000000";
    for (int weapon = 0; weapon <= 2;++weapon) {
        for (int behavior = 3; behavior <= 4;++behavior) { 
            for (int armor = 5; armor <= 7;++armor) { 
                FString enemy = blankenemy;
                enemy[weapon] = (char)((int)enemy[weapon] + 1);
                enemy[behavior] = (char)((int)enemy[behavior] + 1);
                enemy[armor] = (char)((int)enemy[armor] + 1);
                addEnemy(enemy);
            }
        }
    }

}


//wipe previous learnings
void UDriftingBandits::initQtable() {
    for (int i = 0; i < m_EnemyCount;++i) {
        m_Qtable[i] = 0.;
        m_Ntable[i] = 0;
        m_Ttable[i] = 0;
    }
}


FString UDriftingBandits::createEnemy() {
    TArray<double> pvalues = softmax(m_Qtable, m_beta); //softmax on Q values 
    int iRolledTag = rollFromProb(pvalues); //roll according to probability distribution
    return m_Index2Name[iRolledTag];

}

void UDriftingBandits::giveReward(FString enemy, double reward) {
    if (!m_Name2Index.Contains(enemy)) { 
        UE_LOG(LogTemp, Error, TEXT("Could not give reward %f for enemy %s because it is not defined in this AI Model"), reward, *enemy);
        return;
    }
    unsigned int enemyI = *m_Name2Index.Find(enemy);
    m_Ntable[enemyI] ++;
    m_Qtable[enemyI] = m_rho * (1 - pow(m_rho, m_Ntable[enemyI] - 1)) / (1 - pow(m_rho, m_Ntable[enemyI])) * m_Qtable[enemyI] + (1 - m_rho) / (1 - pow(m_rho, m_Ntable[enemyI])) * reward;
    m_Ttable[enemyI]++;
}

FString UDriftingBandits::printQtable() const {
    FString QtableString = "";
    TArray<double> pvalues = softmax(m_Qtable, m_beta); //softmax on Q values 
    for (int i = 0; i < m_EnemyCount;++i) {
        QtableString += m_Index2Name[i] + "   n: " + FString::FromInt(m_Ntable[i]) + "   ,   Q: ";
        if (m_Qtable[i] < 0) QtableString += FString::SanitizeFloat(round(m_Qtable[i] * 10000) / 10000., 4);
        else QtableString += "  " + FString::SanitizeFloat(round(m_Qtable[i] * 10000) / 10000., 4);
        QtableString += "   ,   p: " + FString::SanitizeFloat(round(pvalues[i]*10000)/100.,2) + "%\n";
        
    }
   return QtableString;
}

TMap<FString, double> UDriftingBandits::getQtable() const {
    return TMap<FString, double> {};
}

void UDriftingBandits::setQtable(const TMap<FString, double> Qtable) {
    
}

TArray<double> UDriftingBandits::getQArray() const {
    return m_Qtable;
}

void UDriftingBandits::setQArray(const TArray<double> Qarray) {
    m_Qtable = Qarray;
}

TArray<int> UDriftingBandits::getNArray() const {
    return m_Ntable;
}

void UDriftingBandits::setNArray(const TArray<int> Narray) {
    m_Ntable = Narray;
}

TArray<int> UDriftingBandits::getTArray() const {
    return m_Ttable;
}

void UDriftingBandits::setTArray(const TArray<int> Tarray) {
    m_Ttable = Tarray;
}

TArray<FString> UDriftingBandits::getIndex2Name() const {
    return m_Index2Name;
}

void UDriftingBandits::setIndex2Name(const TArray<FString> i2n) {
    m_Index2Name = i2n;
    m_EnemyCount = 0;
    for (FString enemy : m_Index2Name) {
        m_Name2Index.Add(enemy, m_EnemyCount);
        m_EnemyCount++;
    }
   

}
