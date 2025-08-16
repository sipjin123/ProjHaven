
#pragma once

#include "CoreMinimal.h"
#include "HavenEnums.generated.h"

UENUM(BlueprintType)
enum class EOccupationType : uint8 {
	None = 0			UMETA(DisplayName = "None"),
	Construction = 1	UMETA(DisplayName = "Construction"),
	Miner = 2			UMETA(DisplayName = "Miner"),
};
	