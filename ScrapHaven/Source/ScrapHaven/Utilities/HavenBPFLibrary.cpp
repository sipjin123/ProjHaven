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

TSubclassOf<AActor> UHavenBPFLibrary::GetSpawnClassForItem(const FStoreItem& ItemData)
{
	// Make sure ItemData has a valid row / class reference
	if (!ItemData.ItemActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetSpawnClassForItem: Item '%s' has no SpawnActorClass set!"), *ItemData.ItemName.ToString());
		return nullptr;
	}
	
	return ItemData.ItemActorClass;
}

bool UHavenBPFLibrary::GetFloorInFront(AActor* Actor, FVector& OutFloorLocation, float ForwardDistance, float TraceDistance)
{
	if (!Actor) return false;

	UWorld* World = Actor->GetWorld();
	if (!World) return false;

	// Start point in front of actor
	FVector Start = Actor->GetActorLocation() + Actor->GetActorForwardVector() * ForwardDistance;

	// End point downward
	FVector End = Start - FVector(0, 0, TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Actor);

	bool bHit = World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

#if WITH_EDITOR
	DrawDebugLine(World, Start, End, bHit ? FColor::Green : FColor::Red, false, 2.0f, 0, 2.0f);
#endif

	if (bHit)
	{
		OutFloorLocation = HitResult.Location;
		return true;
	}

	return false;
}