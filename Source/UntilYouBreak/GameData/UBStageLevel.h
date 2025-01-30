#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UBStageLevel.generated.h"

USTRUCT(BlueprintType)
struct FUBStageLevel : public FTableRowBase
{
	GENERATED_BODY()

public:
	FUBStageLevel()
		: EnemyCount(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stage)
	int32 EnemyCount;
};