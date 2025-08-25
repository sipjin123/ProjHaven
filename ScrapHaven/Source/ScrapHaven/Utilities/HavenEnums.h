
#pragma once

#include "CoreMinimal.h"
#include "HavenEnums.generated.h"

UENUM(BlueprintType)
enum class EOccupationType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Construction = 1	UMETA(DisplayName = "Construction"),
	Miner = 2			UMETA(DisplayName = "Miner"),
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
