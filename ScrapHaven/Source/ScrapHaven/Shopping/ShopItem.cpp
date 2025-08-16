// Fill out your copyright notice in the Description page of Project Settings.


#include "Shopping/ShopItem.h"

// Sets default values
AShopItem::AShopItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShopItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShopItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

