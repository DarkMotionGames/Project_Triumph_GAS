// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UMainAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (Keywords = "Kismet"), Category = "MainAbilitySystemLibrary | WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, meta = (Keywords = "Kismet"), Category = "MainAbilitySystemLibrary | WidgetController")
	static UAttributeMenuWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="MainAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category="MainAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="MainAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="MainAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintCallable, Category="MainAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="MainAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InIsBlocked);

	UFUNCTION(BlueprintCallable, Category="MainAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool InIsCritical);

	UFUNCTION(BlueprintCallable, Category="MainAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category="MainAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriendly(const AActor* FirstActor, const AActor* SecondActor);
};
