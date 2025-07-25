// © 2024 DMG. All rights reserved.


#include "MainAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "MainGameplayTags.h"

UMainAssetManager& UMainAssetManager::Get()
{
	check(GEngine)
	UMainAssetManager* MainAssetManager = Cast<UMainAssetManager>(GEngine->AssetManager);
	return *MainAssetManager;
}

void UMainAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FMainGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
} 