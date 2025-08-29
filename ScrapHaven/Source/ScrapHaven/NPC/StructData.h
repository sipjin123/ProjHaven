#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "Utilities/HavenEnums.h"
#include "StructData.generated.h"

USTRUCT(BlueprintType)
struct FDebugStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test")
	int32 ID = 0;
};

USTRUCT(BlueprintType)
struct FPOIData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	FVector OutdoorEntrance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	FVector IndoorEntrance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	int32 UniqueId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	FName DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	EBuildingType BuildingType = EBuildingType::None;

	/** Is the POI data empty? */
	bool IsEmpty() const
	{
		return DisplayName.IsNone() || BuildingType == EBuildingType::None;
	}
};

USTRUCT(BlueprintType)
struct FShopListData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	FPOIData POIData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	TArray<FItemPurchasePair> ShoppingListArray;
};

USTRUCT(BlueprintType)
struct FItemPurchasePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	FName ItemName;
	
	UPROPERTY(BlueprintReadWrite, Category="Coord")
	int32 Quantity = 0;
};
