// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfoDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AttributeName = FText();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadWrite)
	float AttributeValue = 0.0f;
};

/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UAttributeInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAttributeInfo> AttributeInformation;
};