// Fill out your copyright notice in the Description page of Project Settings.


#include "Shopping/ShelfSector.h"

#include "ActorComponents/ShelfManagerComponent.h"

// Sets default values
AShelfSector::AShelfSector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShelfSector::BeginPlay()
{
	Super::BeginPlay();
	ShelfManager = FindComponentByClass<UShelfManagerComponent>();
}

// Called every frame
void AShelfSector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShelfSector::InitializeShelf(FVector NewInteractionCoord)
{
	InteractionCoordinate = NewInteractionCoord;
}

void AShelfSector::RegisterStoreItem(const FStoreItem& StoreItemRef)
{
	StoreItemRefData = StoreItemRef;
}

