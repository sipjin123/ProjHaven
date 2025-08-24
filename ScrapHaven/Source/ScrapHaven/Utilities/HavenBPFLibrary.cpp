// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/HavenBPFLibrary.h"
#include "Shopping/SupplyBox.h"

// BoxSpawnLibrary.cpp
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


ASupplyBox* UHavenBPFLibrary::SpawnSupplyBoxActor(
	UObject* WorldContextObject,
	TSubclassOf<ASupplyBox> BoxClass,
	const FTransform& SpawnTransform,
	FStoreItem ItemData,
	FName BoxName,
	int32 Quantity)
{
	if (!WorldContextObject || !BoxClass) return nullptr;

	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ASupplyBox* NewBox = World->SpawnActor<ASupplyBox>(BoxClass, SpawnTransform, SpawnParams);
	if (NewBox)
	{
		NewBox->CachedItem   = ItemData;
		NewBox->BoxTypeRowName    = BoxName;
		NewBox->Quantity   = Quantity;
	}

	return NewBox;
}
