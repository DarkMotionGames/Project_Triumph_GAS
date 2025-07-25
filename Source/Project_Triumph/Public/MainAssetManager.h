// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "MainAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UMainAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UMainAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
