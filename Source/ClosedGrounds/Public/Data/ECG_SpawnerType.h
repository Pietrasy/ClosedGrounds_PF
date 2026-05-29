#pragma once

UENUM(BlueprintType, Blueprintable)
enum class ESpawnerType : uint8
{
	Null,
	House,
	QuestObject,
	Enemy,
	Fog,
};