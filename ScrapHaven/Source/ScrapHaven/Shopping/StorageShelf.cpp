// Fill out your copyright notice in the Description page of Project Settings.


#include "Shopping/StorageShelf.h"

#include "Subsystems/StoreSubsystem.h"

// Sets default values
AStorageShelf::AStorageShelf()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SlotsRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SlotRoot"));
	RootComponent = Root;
	SlotsRoot->SetupAttachment(Root); // <-- attach it to root
}

void AStorageShelf::BeginPlay()
{
	Super::BeginPlay();
	
	ItemSlots.Empty();

	if (SlotsRoot)
	{
		// Collect all direct children of SlotsRoot
		const TArray<USceneComponent*>& SlotChildren = SlotsRoot->GetAttachChildren();
		for (USceneComponent* ChildComp : SlotChildren)
		{
			if (ChildComp)
			{
				ItemSlots.Add(ChildComp);
				// Draw a debug box at the slot location
				const FVector SlotLocation = ChildComp->GetComponentLocation();
				const FRotator SlotRotation = ChildComp->GetComponentRotation();
				const FVector BoxExtent(20.f, 20.f, 20.f); // size of the box

				DrawDebugBox(
					GetWorld(),             // world
					SlotLocation,           // center
					BoxExtent,              // half size of the box
					SlotRotation.Quaternion(), // rotation
					FColor::Green,          // color
					true,                   // persistent (stays in world)
					-1.f,                   // lifetime (-1 = forever)
					0,                      // depth priority
					2.f                     // line thickness
				);
			}
		}
	}
	
	// Register this shelf with StoreSubsystem (if needed later for lookup)
	if (UGameInstance* GI = GetWorld()->GetGameInstance())
	{
		if (UStoreSubsystem* Store = GI->GetSubsystem<UStoreSubsystem>())
		{
			// You could store this shelf reference if you want centralized tracking
			Store->RegisterShelfActor(this);
		}
	}
	
	// Ensure PlacedItems matches slot count
	PlacedItems.SetNum(ItemSlots.Num());
}

UStoreSubsystem* GetStoreSubsystem(UWorld* World)
{
	return (World && World->GetGameInstance()) 
		? World->GetGameInstance()->GetSubsystem<UStoreSubsystem>() 
		: nullptr;
}

bool AStorageShelf::AddItemToSlot(const FName& ItemName)
{
	if (ItemName.IsNone()) return false;

	UE_LOG(LogTemp, Log, TEXT("Looking for free Slot for: %s - %d"), *ItemName.ToString(), PlacedItems.Num());
	for (int32 i = 0; i < PlacedItems.Num(); i++)
	{
		if (PlacedItems[i].ItemName.IsNone()) // free slot
		{
			FStoreItem NewItem;
			NewItem.ItemName = ItemName;
			//NewItem.UniqueId = FMath::Rand(); // optional

			PlacedItems[i] = NewItem;

			UE_LOG(LogTemp, Log, TEXT("Adding to Slot: %s - %d/%d"), *ItemName.ToString(), i, PlacedItems.Num());
			// TODO: spawn a box mesh/actor at ItemSlots[i]->GetComponentTransform()
			// Ask subsystem to spawn the physical box
			if (UGameInstance* GI = GetWorld()->GetGameInstance())
			{
				if (UStoreSubsystem* Store = GI->GetSubsystem<UStoreSubsystem>())
				{
					if (ItemSlots.IsValidIndex(i) && ItemSlots[i])
					{
						FTransform SlotTransform = ItemSlots[i]->GetComponentTransform();
						Store->SpawnItemBox(GetWorld(), NewItem, SlotTransform);
					}
				}
			}
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Skip Slot: %s - %d"), *ItemName.ToString(), i);
		}
	}

	// No free slot
	return false;
}

bool AStorageShelf::RemoveItemFromSlot(int32 SlotIndex)
{
	if (!PlacedItems.IsValidIndex(SlotIndex)) return false;

	if (!PlacedItems[SlotIndex].ItemName.IsNone())
	{
		// TODO: destroy the box mesh/actor attached to this slot

		PlacedItems[SlotIndex] = FStoreItem(); // reset
		return true;
	}

	return false;
}

void AStorageShelf::LogShelfContents() const
{
	UE_LOG(LogTemp, Log, TEXT("Shelf [%s] contents:"), *GetName());
	if (ShelfInventory.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("  [Empty]"));
	}
	else
	{
		for (const FStoreItem& Item : ShelfInventory)
		{
			UE_LOG(LogTemp, Log, TEXT("  Box of: %s "),
				*Item.ItemName.ToString());
		}
	}
}