#include "SupplyBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"

ASupplyBox::ASupplyBox()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Mesh
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(Root);

	DynamicCollider = nullptr;
	Quantity = 0;
}

void ASupplyBox::BeginPlay()
{
	Super::BeginPlay();

	// Load item data from DataTable
	if (ItemDataTable && ItemRowName != NAME_None)
	{
		if (const FStoreItem* Row = ItemDataTable->FindRow<FStoreItem>(ItemRowName, TEXT("SupplyBox Init")))
		{
			CachedItem = *Row;
			Quantity = 20; // default fill
			OnSupplyChanged.Broadcast(CachedItem, Quantity);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SupplyBox %s: Could not find item row '%s' in DataTable."), *GetName(), *ItemRowName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SupplyBox %s: No ItemDataTable or ItemRowName set."), *GetName());
	}
}

bool ASupplyBox::AddItem(const FStoreItem& Item)
{
	// Only allow adding if it's the same type as this box
	if (Item.ItemName.EqualTo(CachedItem.ItemName))
	 // assuming FStoreItem has ItemName field
	{
		Quantity++;
		OnSupplyChanged.Broadcast(CachedItem, Quantity);
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("SupplyBox %s: Tried to add wrong item type (%s)"), *GetName(), *Item.ItemName.ToString());
	return false;
}

bool ASupplyBox::TakeItem(FStoreItem& OutItem)
{
	if (Quantity <= 0)
	{
		return false;
	}

	OutItem = CachedItem; // return a copy of the cached item
	Quantity--;

	OnSupplyChanged.Broadcast(CachedItem, Quantity);
	return true;
}
