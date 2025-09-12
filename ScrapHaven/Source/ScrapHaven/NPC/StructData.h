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
struct FPOIBasic
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	int32 UniqueId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	EBuildingType BuildingType = EBuildingType::None;

	/** Is the POI data empty? */
	bool IsEmpty() const
	{
		return BuildingType == EBuildingType::None;
	}
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
struct FItemPurchasePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	int32 Quantity = 0;
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
struct FNPCData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Coord")
	FName CitizenName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	ENPCType NPCType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Status")
	EOccupationType OccupationType;
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;
};