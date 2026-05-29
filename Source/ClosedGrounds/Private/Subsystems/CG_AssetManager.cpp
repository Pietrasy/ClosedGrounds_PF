// Pietras


#include "Subsystems/CG_AssetManager.h"

const FPrimaryAssetType UCG_AssetManager::QuestType = TEXT("Quest");

UCG_AssetManager& UCG_AssetManager::Get()
{
	UCG_AssetManager* Manager = Cast<UCG_AssetManager>(GEngine->AssetManager);

	if (!Manager)
	{
		UE_LOG(LogTemp, Error, TEXT("AssetManager is NOT UCG_AssetManager!"));
		check(Manager); // crash with info instead of silent UB
	}

	return *Manager;
}
