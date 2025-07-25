// © 2024 DMG. All rights reserved.


#include "AbilitySystem/Data/AttributeInfoDataAsset.h"

FAttributeInfo UAttributeInfoDataAsset::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Attribute not found, Tag: [%s] on AttributeInfoDA."), *AttributeTag.ToString());	
	}
	return FAttributeInfo();
}
