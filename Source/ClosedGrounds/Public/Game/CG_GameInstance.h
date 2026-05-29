// Pietras

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CG_GameInstance.generated.h"

class UCG_QuestManager;

UCLASS()
class CLOSEDGROUNDS_API UCG_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
};