// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemComponentBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTag, const FGameplayTagContainer& /* Asset Tags */);
/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UAbilitySystemComponentBase : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();	
	FEffectAssetTag EffectAssetTag;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const;


};
