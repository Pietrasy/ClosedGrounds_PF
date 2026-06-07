## Initialize gameplay after loaded quests from Asset Manager
Loading quests is essential for initializing gameplay, as its elements depend on the quest properties.
We retrieve the primary asset IDs from the asset manager and load them asynchronously into memory. Then, we call the CacheLoadedQuests function, which adds the quest primary asset object to the AllQuests array and a flag indicating that the quests have been loaded.
```cpp
void UCG_QuestManager::InitializeQuestSystem()
{

	// [...]

	// Get quest primary asset IDs from Asset Manager.
	TArray<FPrimaryAssetId> QuestPrimaryAssets;
	UAssetManager::Get().GetPrimaryAssetIdList(FPrimaryAssetType("Quest"), QuestPrimaryAssets);
	
	// [...]
	
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Loading %d quest assets..."), __FUNCTION__, QuestPrimaryAssets.Num());

	// Asynchronously loading quests as primary objects.
	const FStreamableDelegate OnAsyncAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &UCG_QuestManager::CacheLoadedQuests);
	UAssetManager::Get().LoadPrimaryAssets(QuestPrimaryAssets, TArray<FName>(), OnAsyncAssetsLoadedDelegate);
}

void UCG_QuestManager::CacheLoadedQuests()
{
	UE_LOG(LogQuestManager, Log, TEXT("[%hs] - Quests have been loaded!"), __FUNCTION__);
	
	TArray<UObject*> ObjectList;
	//Get loaded quest primary assets from Asset Manager.
	UAssetManager::Get().GetPrimaryAssetObjectList(FPrimaryAssetType("Quest"), ObjectList);
	
	// Add quests to AllQuests array.
	for (UObject* Element : ObjectList)
	{
		AllQuests.Add(Cast<UCG_QuestData>(Element));
	}
	
	// Set a flag indicating that the quests have been loaded.
	bQuestLoaded = true;
	RollQuests();
}
```
In the InitGame of the GameMode class, we start by initializing the DayManager component, which is responsible for handling gameplay.
```cpp
void ACG_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	// [...]
	
	// Initialize a Day Manager.
	DayManagerComponent->InitializeDayManager();
}
```
The InitializeDay function checks whether in QuestManager have already been loaded into QuestManager. If not, it binds to the OnActivatedTodayQuests delegate. Once the tasks are loaded into memory, we initialize the gameplay
```cpp
void UCG_DayManagerComponent::InitializeDayManager()
{
	// [...]
	
	UCG_QuestManager* QuestManager = CGGameMode->GetQuestManager();

	if (!QuestManager)
	{
		UE_LOG(LogGame, Error, TEXT("[%hs] - Invalid QuestManager!"), __FUNCTION__);
		return;
	}
	
	// Check whether the quests from QuestManager have been loaded. If not, bind to its delegate.
	if (!QuestManager->AreQuestsLoaded())
	{
		QuestManager->OnActivatedTodayQuests.BindUFunction(this, TEXT("InitializeDay"));
		return;
	}
	
	InitializeDay();
}

void UCG_DayManagerComponent::InitializeDay()
{
	// [...]
	
	// Call the delegate when the DayManager is ready.
	OnInitializedDay.ExecuteIfBound();
	UE_VLOG_UELOG(this, LogDayManager, Log, TEXT("[%hs] - Initialized day."), __FUNCTION__);
}
}
```
