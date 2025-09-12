// Fill out your copyright notice in the Description page of Project Settings.


#include "Shopping/ShopInteractable.h"

// Sets default values
AShopInteractable::AShopInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Make sure actor has a root (usually DefaultSceneRoot from editor)
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	}

	// Create SlotsRoot and attach it to whatever root exists
	NPCInteractionRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SlotsRoot"));
	NPCInteractionRoot->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShopInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShopInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

