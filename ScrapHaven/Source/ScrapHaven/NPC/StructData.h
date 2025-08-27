#pragma once

#include "CoreMinimal.h"
#include "UObject/NameTypes.h"
#include "Utilities/HavenEnums.h"
#include "StructData.generated.h"

USTRUCT(BlueprintType)
struct FPOIData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category="Coord")
	FVector OutdoorEntrance;

	UPROPERTY(BlueprintReadWrite, Category="Coord")
	FVector IndoorEntrance;

	UPROPERTY(BlueprintReadWrite, Category="Coord")
	int32 UniqueId = 0;

	UPROPERTY(BlueprintReadWrite, Category="Coord")
	FName DisplayName;

	UPROPERTY(BlueprintReadWrite, Category="Coord")
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


	
	UPROPERTY(BlueprintReadWrite, Category="Coord")
	FVector OutdoorEntrance;

	UPROPERTY(BlueprintReadWrite, Category="Coord")
	FVector IndoorEntrance;

	UPROPERTY(BlueprintReadWrite, Category="Coord")
	int32 UniqueId = 0;

	UPROPERTY(BlueprintReadWrite, Category="Coord")
	FName DisplayName;

	UPROPERTY(BlueprintReadWrite, Category="Coord")
	EBuildingType BuildingType = EBuildingType::None;

	/** Is the POI data empty? */
	bool IsEmpty() const
	{
		return DisplayName.IsNone() || BuildingType == EBuildingType::None;
	}
};