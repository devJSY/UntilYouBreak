#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UBCharacterExp.generated.h"

USTRUCT(BlueprintType)
struct FUBCharacterExp : public FTableRowBase
{
	GENERATED_BODY()

public:
	FUBCharacterExp()
		: RewardExp(0), MaxExp(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Exp)
	int32 RewardExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Exp)
	int32 MaxExp;
};