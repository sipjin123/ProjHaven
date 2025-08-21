#include "ShelfManagerComponent.h"

#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Data/FStoreItem.h"
#include "GameFramework/Actor.h"
#include "Shopping/ShelfSector.h"

UShelfManagerComponent::UShelfManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UShelfManagerComponent::InitializeShelf(UBoxComponent* ShelfBox, UBoxComponent* ItemBox, AShelfSector* ShelfSector, const FStoreItem& StoreItemRef)
{
    if (!ShelfBox || !ItemBox || !ShelfSector) return;

    ShelfSector->StoreItemRefData = StoreItemRef;
    ShelfReference = ShelfBox;
    ItemReference  = ItemBox;

    // Shelf extents
    const FVector ShelfLocalExtent = ShelfBox->GetUnscaledBoxExtent();
    const FVector ShelfScaleAbs    = ShelfBox->GetComponentTransform().GetScale3D().GetAbs();
    const FVector ShelfWorldExtent = ShelfLocalExtent * ShelfScaleAbs;

    // Item extents
    const FVector ItemWorldExtent =
        ItemBox->GetUnscaledBoxExtent() * ItemBox->GetComponentTransform().GetScale3D().GetAbs();

    // Fit counts
    ItemsPerRow    = FMath::Max(1, FMath::FloorToInt((ShelfWorldExtent.X * 2.f) / (ItemWorldExtent.X * 2.f)));
    ItemsPerColumn = FMath::Max(1, FMath::FloorToInt((ShelfWorldExtent.Y * 2.f) / (ItemWorldExtent.Y * 2.f)));
    MaxLayers      = FMath::Max(1, FMath::FloorToInt((ShelfWorldExtent.Z * 2.f) / (ItemWorldExtent.Z * 2.f)));

    Slots.Empty();

    const FTransform BoxXform = ShelfBox->GetComponentTransform();

    auto SafeDiv = [](float n, float d) { return (d != 0.f) ? (n / d) : 0.f; };

    // Convert item WORLD extents to LOCAL shelf space
    const FVector InsetLocal(
        SafeDiv(ItemWorldExtent.X, ShelfScaleAbs.X),
        SafeDiv(ItemWorldExtent.Y, ShelfScaleAbs.Y),
        SafeDiv(ItemWorldExtent.Z, ShelfScaleAbs.Z));

    const FVector StepLocal(
        SafeDiv(ItemWorldExtent.X * 2.f, ShelfScaleAbs.X),
        SafeDiv(ItemWorldExtent.Y * 2.f, ShelfScaleAbs.Y),
        SafeDiv(ItemWorldExtent.Z * 2.f, ShelfScaleAbs.Z));

    // ✅ Start at the bottom face of the shelf instead of center
    FVector StartLocal = -ShelfLocalExtent + InsetLocal;

    for (int32 Layer = 0; Layer < MaxLayers; ++Layer)
    {
        for (int32 Row = 0; Row < ItemsPerRow; ++Row)      // X axis
        {
            for (int32 Col = 0; Col < ItemsPerColumn; ++Col) // Y axis
            {
                FVector LocalPos = StartLocal + FVector(Row * StepLocal.X,
                                                        Col * StepLocal.Y,
                                                        Layer * StepLocal.Z);

                // ✅ Shift down by half item height so bottom sits on slot
                LocalPos.Z -= InsetLocal.Z;

                FItemSlot NewSlot;
                NewSlot.WorldLocation = BoxXform.TransformPosition(LocalPos);
                Slots.Add(NewSlot);
            }
        }
    }
    DrawDebugBox(GetWorld(), ShelfBox->GetComponentLocation(), ShelfLocalExtent, ShelfBox->GetComponentQuat(), FColor::Yellow, false, 10.f);
    for (const auto& S : Slots) DrawDebugPoint(GetWorld(), S.WorldLocation, 8.f, FColor::Green, false, 10.f);
}

bool UShelfManagerComponent::PlaceItemInNextSlot(AActor* ItemActor)
{
    if (!ItemActor) return false;

    for (FItemSlot& Slot : Slots)
    {
        if (!Slot.bOccupied)
        {
            ItemActor->SetActorLocation(Slot.WorldLocation);
            Slot.bOccupied = true;
            Slot.OccupyingItem = ItemActor;
            TotalItems++;
            return true;
        }
    }

    return false; // Shelf full
}

void UShelfManagerComponent::ClearShelf()
{
    for (FItemSlot& Slot : Slots)
    {
        Slot.bOccupied = false;
    }
}

AActor* UShelfManagerComponent::TakeItem(int32 SlotIndex)
{
    if (Slots.IsValidIndex(SlotIndex))
    {
        FItemSlot& Slot = Slots[SlotIndex];
        if (Slot.bOccupied && Slot.OccupyingItem.IsValid())
        {
            AActor* Item = Slot.OccupyingItem.Get();

            // Free the slot
            Slot.bOccupied = false;
            Slot.OccupyingItem = nullptr;
            TotalItems--;
            return Item; // Caller now owns the item reference
        }
    }
    return nullptr; // No valid item
}

AActor* UShelfManagerComponent::TakeLastItem()
{
    for (int32 i = Slots.Num() - 1; i >= 0; --i) // iterate backwards
    {
        FItemSlot& Slot = Slots[i];
        if (Slot.bOccupied && Slot.OccupyingItem.IsValid())
        {
            AActor* Item = Slot.OccupyingItem.Get();

            // Free the slot
            Slot.bOccupied = false;
            Slot.OccupyingItem = nullptr;
            TotalItems--;
            return Item; // Return the item reference
        }
    }
    return nullptr; // No items found
}