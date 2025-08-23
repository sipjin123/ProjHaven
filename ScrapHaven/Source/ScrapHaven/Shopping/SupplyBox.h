#pragma once

#include "CoreMinimal.h"
#include "ShopItem.h"   // contains FStoreItem
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "SupplyBox.generated.h"

class UStaticMeshComponent;
class UShapeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSupplyChanged, const FStoreItem&, ItemData, int32, NewCount);

UCLASS()
class SCRAPHAVEN_API ASupplyBox : public AActor
{
	GENERATED_BODY()

public:
	ASupplyBox();

protected:
	virtual void BeginPlay() override;

	/** Cached item definition this box is storing */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Inventory")
	FStoreItem CachedItem;

public:
	/** Visual mesh for the box (can be swapped in BP) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* BoxMesh;

	/** Collider you can assign dynamically in Blueprint */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collision")
	UShapeComponent* DynamicCollider;

	/** Data table reference where all store items are defined */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	UDataTable* ItemDataTable;

	/** Row name of the item this box should contain */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	FName ItemRowName;

	/** Event when items change */
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnSupplyChanged OnSupplyChanged;

	/** Add an item into the box */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(const FStoreItem& Item);

	/** Take out one item from the box */
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool TakeItem(FStoreItem& OutItem);

	/** Get number of items currently in the box */
	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetItemCount() const { return Quantity; }

	/** Check if the box is empty */
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool IsEmpty() const { return Quantity <= 0; }

	/** Get the cached item type for this box */
	UFUNCTION(BlueprintPure, Category="Inventory")
	const FStoreItem& GetItemType() const { return CachedItem; }
	
	/** How many items are currently inside */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Inventory")
	int32 Quantity = 0;
};
