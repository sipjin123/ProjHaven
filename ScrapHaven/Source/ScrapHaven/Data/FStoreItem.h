#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCitizenAttributes.h"
#include "FCitizenNeedsModifiers.h"
#include "FStoreItem.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0        UMETA(DisplayName="None"),
	Food = 1        UMETA(DisplayName="Food"),
	Medicine = 2    UMETA(DisplayName="Medicine"),
	Tool = 3        UMETA(DisplayName="Tool"),
	Material = 4    UMETA(DisplayName="Material"),
	Luxury = 5      UMETA(DisplayName="Luxury"),
	Parts = 7       UMETA(DisplayName="Parts"),
	Other = 8       UMETA(DisplayName="Other")
};

USTRUCT(BlueprintType)
struct FStoreItem : public FTableRowBase
{
	GENERATED_BODY()

public:
	/** Display name for UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText ItemName;

	/** Optional description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText Description;

	/** Icon for UI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* Icon = nullptr;

	/** Item type (food, medicine, etc.) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType = EItemType::Other;

	/** Base buy price in the shop */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy", meta=(ClampMin="0"))
	float BuyPrice = 0;

	/** Base sell price if player sells back */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy", meta=(ClampMin="0"))
	float SellPrice = 0;

	/** Whether the item can stack (e.g. food, materials) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bStackable = false;

	/** Max stack size if stackable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(EditCondition="bStackable", ClampMin="1", ClampMax="999"))
	int32 MaxStackSize = 1;

	/** If item modifies citizen needs (e.g. food reduces hunger, medicine restores health) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	FCitizenAttributes AttributeEffect;
};