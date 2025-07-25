// © 2024 DMG. All rights reserved.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/AttributeSetBase.h"
#include "Player/PlayerControllerBase.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAttributeSetBase* AttributeSetBase = CastChecked<UAttributeSetBase>(AttributeSet);
	OnHealthChanged.Broadcast(AttributeSetBase->GetHealth());
	OnMaxHealthChanged.Broadcast(AttributeSetBase->GetMaxHealth());
	OnStaminaChanged.Broadcast(AttributeSetBase->GetStamina());
	OnMaxStaminaChanged.Broadcast(AttributeSetBase->GetMaxStamina());
	OnAuraChanged.Broadcast(AttributeSetBase->GetAura());
	OnMaxAuraChanged.Broadcast(AttributeSetBase->GetMaxAura());

	Cast<UAbilitySystemComponentBase>(AbilitySystemComponent)->EffectAssetTag.AddLambda
	(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				// TODO: Broadcast Tags to Widget Controller for HUD use.
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUserInterfaceWidgetRow* Row = GetDataTableRowByTag<FUserInterfaceWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
				
			}
		}
	);
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAttributeSetBase* AttributeSetBase = CastChecked<UAttributeSetBase>(AttributeSet);
	APlayerControllerBase* PlayerControllerBase = CastChecked<APlayerControllerBase>(PlayerController);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddLambda
	(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddLambda
	(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddLambda
	(
		[this](const FOnAttributeChangeData& Data)
		{
			OnStaminaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute()).AddLambda
	(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxStaminaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetAuraAttribute()).AddLambda
	(
		[this](const FOnAttributeChangeData& Data)
		{
			OnAuraChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxAuraAttribute()).AddLambda
	(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxAuraChanged.Broadcast(Data.NewValue);
		}
	);

	//Call Tab key pressed callback
	if (PlayerControllerBase)
	{
		PlayerControllerBase->TabKeyPressedDelegate.AddLambda
		(
			[this]()
			{
				OnTabKeyPressed.Broadcast();
			}
		);
	}
}