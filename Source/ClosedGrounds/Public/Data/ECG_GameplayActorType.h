#pragma once

UENUM(BlueprintType, Blueprintable)
enum class EGameplayActorType : uint8
{
	Null,
	HouseActor,
	QuestActor,
	EnemySpawner,
	FogZoneActor,
};