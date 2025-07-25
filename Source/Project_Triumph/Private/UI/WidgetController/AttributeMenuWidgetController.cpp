// © 2024 DMG. All rights reserved.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AttributeSetBase.h"
#include "AbilitySystem/Data/AttributeInfoDataAsset.h"


void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAttributeSetBase* AttributeSetBase = CastChecked<UAttributeSetBase>(AttributeSet);
	check(AttributeInfo);
	for (auto& Pair : AttributeSetBase->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAttributeSetBase* AttributeSetBase = CastChecked<UAttributeSetBase>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AttributeSetBase->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

