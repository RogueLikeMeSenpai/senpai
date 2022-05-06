// Fill out your copyright notice in the Description page of Project Settings.


#include "Test_ActorComponent_AIModel.h"

// Sets default values for this component's properties
UTest_ActorComponent_AIModel::UTest_ActorComponent_AIModel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTest_ActorComponent_AIModel::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTest_ActorComponent_AIModel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTest_ActorComponent_AIModel::MySuperDuperCoolFunction()
{
	UE_LOG(LogTemp, Display, TEXT("You called my super duper cool function! gz"));
}

