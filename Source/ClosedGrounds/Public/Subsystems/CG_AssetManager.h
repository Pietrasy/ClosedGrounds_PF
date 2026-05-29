// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CG_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class CLOSEDGROUNDS_API UCG_AssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UCG_AssetManager& Get();
	
	static const FPrimaryAssetType QuestType;
};
