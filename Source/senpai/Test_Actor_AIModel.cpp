// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_Actor_AIModel.h"

// Sets default values
ATest_Actor_AIModel::ATest_Actor_AIModel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATest_Actor_AIModel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATest_Actor_AIModel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATest_Actor_AIModel::MyCoolFunction()
{
	UE_LOG(LogTemp, Display, TEXT("You called my cool function! gz"));
}
