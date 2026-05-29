// Pietras

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogQuestManager, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogDayManager, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWeaponComponent, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogSaveSystem, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGAS, All, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGame, All, All);

#if !UE_BUILD_SHIPPING
#define LOG_ERROR(x, ...) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(x), ##__VA_ARGS__));} \
UE_LOG(LogTemp, Error, TEXT(x), ##__VA_ARGS__);
#else
#define LOG_ERROR(x, ...)
#endif


