// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

class UInputAction;
struct FGameplayTag;

USTRUCT(BlueprintType)
struct FInputActionConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag InputTag = FGameplayTag();
};
/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<FInputActionConfig> AbilityInputActions;
	
};
