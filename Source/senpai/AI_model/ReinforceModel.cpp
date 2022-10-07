// Fill out your copyright notice in the Description page of Project Settings.


#include "ReinforceModel.h"

// Sets default values for this component's properties
UReinforceModel::UReinforceModel() 
    : m_Qtable({}), m_LayerCount(3), m_TagCount(8), m_maxQvalue(5.), m_minQvalue(-5.)
{
    m_Layers.Init(FNestedIntArray(), 3);
    blankEnemy = "00000000";
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    add2Layer(0, { 0,1,2 });
    add2Layer(1, { 3,4 });
    add2Layer(2, { 5,6,7 });
    initQtable();
}


// Called when the game starts
void UReinforceModel::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UReinforceModel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}




void UReinforceModel::add2Layer(unsigned short iLayer, unsigned int tagNumber) {
    assert(iLayer < m_LayerCount);
    assert(tagNumber < m_TagCount);
    for (int tag : m_Layers[iLayer].A) { //check if tag is already part of Layer
        if (tagNumber == tag) return;
    }
    m_Layers[iLayer].A.Add(tagNumber);
}

void UReinforceModel::add2Layer(unsigned short iLayer, TArray<unsigned int> tagNumbers) {
    for (unsigned int tagNumber : tagNumbers) {
        add2Layer(iLayer, tagNumber);
    }
}

//create all possible enemies in Qtable
void UReinforceModel::initQtable() {
    TArray<FNestedStringArray> possEnemy;
    possEnemy.Init(FNestedStringArray(), m_LayerCount + 1);
    possEnemy[0].A.Add(blankEnemy);
    m_Qtable.Add(blankEnemy,0.);
    for (int i = 0; i < m_LayerCount;++i) {
        for (int tag : m_Layers[i].A) { // apply each action on enemy
            for (FString enemy : possEnemy[i].A) {
                enemy[tag] = (char)((int)enemy[tag] + 1); //increase number at position
                possEnemy[i + 1].A.Add(enemy);
                m_Qtable.Add(enemy,0);
            }
        }
    }
}

TMap<FString, double> UReinforceModel::getQtable() const {
    return m_Qtable;
}

void UReinforceModel::setQtable(TMap<FString, double> Qtable) {
    assert(checkQtableValid(Qtable));
    m_Qtable = Qtable;
}


FString UReinforceModel::createEnemy() {
    FString enemy = blankEnemy;
    FString enemyTmp;
    for (int i = 0; i < m_LayerCount;++i) {
        TArray<double> Qvalues; //Q values of all possible next enemies
        for (int tag : m_Layers[i].A) {
            enemyTmp = enemy;
            enemyTmp[tag] = (char)((int)enemyTmp[tag] + 1); //increase number at position
            Qvalues.Add(*m_Qtable.Find(enemyTmp));
        }
        TArray<double> pvalues = softmax(Qvalues, 1); //softmax on Q values 
        int iRolledTag = rollFromProb(pvalues); //roll according to probability distribution
        int rolledTag = m_Layers[i].A[iRolledTag];
        enemy[rolledTag] = (char)((int)enemy[rolledTag] + 1);
    }
    
    return FString(*enemy);
  
}

void UReinforceModel::giveReward(const FString enemyIn, double reward) {
    //TODO: bias and learning rate for reward
    assert(QtableContains(enemyIn));
    TArray<FNestedStringArray> possEnemy;
    possEnemy.Init(FNestedStringArray(), m_LayerCount + 1);
    possEnemy[m_LayerCount].A.Add(enemyIn);
    float newQvalue = *m_Qtable.Find(enemyIn) + reward;
    if (newQvalue > m_maxQvalue) newQvalue = m_maxQvalue;
    if (newQvalue < m_minQvalue) newQvalue = m_minQvalue;
    m_Qtable.Add(enemyIn, newQvalue);
    for (int i = m_LayerCount - 1; i >= 0;--i) { // propagate backwards
        for (int tag_back : m_Layers[i].A) { // revert each action on enemy
            for (FString enemy : possEnemy[i + 1].A) {
                enemy[tag_back] = (char)((int)enemy[tag_back] - 1); //decrease number at position
                if (QtableContains(enemy)) { // check if reverting tag yields possible enemy
                    // check if already in possible enemies
                    if (!possEnemy[i].A.Contains(enemy)) {
                        possEnemy[i].A.Add(enemy);
                    }
                    TArray<double> Qvalues; //Q values of all possible next enemies
                    for (int tag_forward : m_Layers[i].A) {
                        FString enemyTmp = enemy;
                        enemyTmp[tag_forward] = (char)((int)enemyTmp[tag_forward] + 1); //increase number at position
                        Qvalues.Add(*m_Qtable.Find(enemyTmp));
                    }
                    double maxQvalue = Qvalues[0];
                    for (double Qvalue : Qvalues) {
                        if (Qvalue > maxQvalue) maxQvalue = Qvalue;
                    }
                    m_Qtable.Add(enemy, maxQvalue);
                }
                
            }
        }
    }
}

FString UReinforceModel::printQtable() const {
    FString QtableString = "";
    TArray<FNestedStringArray> possEnemy;
    possEnemy.Init(FNestedStringArray(), m_LayerCount + 1);
    for (auto const& pair : m_Qtable) {
        FString enemy = pair.Key;
        int activeTags = 0;
        for (auto c : enemy) {
            activeTags += ((int)c - (int)'0');
        }
        FString entry = "{" + pair.Key + ": " + FString::SanitizeFloat(pair.Value) + "}\n";
        possEnemy[activeTags].A.Add(entry);
    }
    for (int i = 0; i <= m_LayerCount;++i) {

        for (FString entry : possEnemy[i].A) {
            for (int j = 0; j < i;++j) {
                QtableString += "    ";
            }
            QtableString += entry;
        }
    }
    QtableString += "\n";
    return QtableString;
}


bool UReinforceModel::QtableContains(FString enemy) {
    return (m_Qtable.Contains(enemy));
}

bool UReinforceModel::checkQtableValid(const TMap<FString, double> Qtable) const {
    TArray<FNestedStringArray> possEnemy;
    possEnemy.Init(FNestedStringArray(), m_LayerCount + 1);
    possEnemy[0].A.Add(blankEnemy);
    for (int i = 0; i < m_LayerCount;++i) {
        for (int tag : m_Layers[i].A) { // apply each action on enemy
            for (FString enemy : possEnemy[i].A) {
                enemy[tag] = (char)((int)enemy[tag] + 1); //increase number at position
                possEnemy[i + 1].A.Add(enemy);
                if (!Qtable.Contains(enemy)) {
                    return false; // Qtable does not contain a possible enemy, therefore invalid
                }
            }
        }
    }
    return true;
}


TArray<double> UReinforceModel::getQArray() const {
    return TArray<double> {};
}

void UReinforceModel::setQArray(const TArray<double> Qarray) {
    
}

TArray<int> UReinforceModel::getNArray() const {
    return TArray<int> {};
}

void UReinforceModel::setNArray(const TArray<int> Narray) {
   
}

TArray<int> UReinforceModel::getTArray() const {
    return TArray<int> {};
}

void UReinforceModel::setTArray(const TArray<int> Tarray) {
    
}

TArray<FString> UReinforceModel::getIndex2Name() const {
    return TArray<FString> {};
}

void UReinforceModel::setIndex2Name(const TArray<FString> i2n) {

}




