
#pragma once

#include "CoreMinimal.h"
#include "HavenEnums.generated.h"

UENUM(BlueprintType)
enum class EOccupationType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Construction = 1	UMETA(DisplayName = "Construction"),
	Miner = 2			UMETA(DisplayName = "Miner"),
	Government = 3			UMETA(DisplayName = "Government"),
	Vendor = 4			UMETA(DisplayName = "Vendor"),
	Scavenger = 5			UMETA(DisplayName = "Scavenger"),
	Civilian = 6			UMETA(DisplayName = "Civilian"),
};

UENUM(BlueprintType)
enum class EDroneTask : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Cleaning = 1	UMETA(DisplayName = "Cleaning"),
	Supplying = 2			UMETA(DisplayName = "Supplying"),
	Farming = 3			UMETA(DisplayName = "Farming"),
	Delivery = 4			UMETA(DisplayName = "Delivery"),
	Scavenging = 5			UMETA(DisplayName = "Scavenging"),
};

UENUM(BlueprintType)
enum class EBuildingType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Haven = 1	UMETA(DisplayName = "Haven"),
	House  = 2	UMETA(DisplayName = "House"),
	TownHall  = 3	UMETA(DisplayName = "TownHall"),
	Smithy  = 4	UMETA(DisplayName = "Smithy"),
	Hospital  = 5	UMETA(DisplayName = "Hospital"),
	Market  = 6	UMETA(DisplayName = "Market"),
	Storage = 7	UMETA(DisplayName = "Storage"),
	Tavern = 8	UMETA(DisplayName = "Tavern"),
};

UENUM(BlueprintType)
enum class ENPCBehaviorType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Hungry = 1	UMETA(DisplayName = "Hungry"),
	Grumpy  = 2	UMETA(DisplayName = "Grumpy"),
	Thirsty  = 3	UMETA(DisplayName = "Thirsty"),
	Stressed  = 4	UMETA(DisplayName = "Stressed"),
	Dissociate  = 5	UMETA(DisplayName = "Dissociate"),
	Curious  = 6	UMETA(DisplayName = "Curious"),
	Happy = 7	UMETA(DisplayName = "Happy"),
	Sociable = 8	UMETA(DisplayName = "Sociable"),
};
