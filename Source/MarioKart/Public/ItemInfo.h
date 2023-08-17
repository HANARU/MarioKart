#pragma once

#include "Engine/DataTable.h"
#include "ItemInfo.generated.h"

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Item_Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Item_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTargetForSelf;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Item_Texture;
};