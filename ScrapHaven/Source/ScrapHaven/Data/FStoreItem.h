#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCitizenAttributes.h"
#include "FCitizenNeedsModifiers.h"
#include "Utilities/HavenEnums.h"
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
	// --- Basic Info ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	EItemType ItemType = EItemType::Other;

	UPROPERTY(BlueprintReadWrite, Category="Item")
	EFoodCategory Category = EFoodCategory::None;

	// --- Economy ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy", meta=(ClampMin="0"))
	float BuyPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy", meta=(ClampMin="0"))
	float SellPrice = 0;

	// --- Stack ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(EditCondition="bStackable", ClampMin="1", ClampMax="999"))
	int32 MaxStackSize = 1;

	// --- Effects on citizen ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects", meta=(ClampMin="0.0", ClampMax="100.0"))
	int32 HungerEffect = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects", meta=(ClampMin="0.0", ClampMax="100.0"))
	int32 ThirstEffect = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects", meta=(ClampMin="0.0", ClampMax="100.0"))
	int32 EnergyEffect = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects", meta=(ClampMin="0.0", ClampMax="100.0"))
	int32 HealthEffect = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects", meta=(ClampMin="0.0", ClampMax="100.0"))
	int32 HappinessEffect = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects", meta=(ClampMin="0.0", ClampMax="100.0"))
	int32 SocialEffect = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects", meta=(ClampMin="0.0", ClampMax="100.0"))
	int32 ComfortEffect = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects", meta=(ClampMin="0"))
	int32 WealthEffect = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InGameUse", meta=(ClampMin="0"))
	int32 Quantity;
	
	// --- Optional gameplay flags ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bIsEssential = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bIsTreat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	TSubclassOf<class AShopItem> ItemActorClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	bool bUnlocked = true;
	
	// --- Helper functions ---
	bool IsTreat() const { return bIsTreat; }
	bool IsEssential() const { return bIsEssential; }

	FCitizenAttributes GetAttributeEffect() const
	{
		FCitizenAttributes Attr;
		Attr.Hunger = HungerEffect;
		Attr.Energy = EnergyEffect;
		Attr.Health = HealthEffect;
		Attr.Happiness = HappinessEffect;
		Attr.Social = SocialEffect;
		Attr.Thirst = ThirstEffect;
		Attr.Wealth = WealthEffect;
		return Attr;
	}
};