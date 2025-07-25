// © 2024 DMG. All rights reserved.


#include "AbilitySystem/MainAbilitySystemLibrary.h"

#include "MainAbilitySystemTypes.h"
#include "Engine/OverlapResult.h"
#include "Game/BaseGameMode.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerStateBase.h"
#include "UI/WidgetController/MainWidgetController.h"
#include "UI/HUD/MainHUD.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

UOverlayWidgetController* UMainAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD()))
		{
			APlayerStateBase* PlayerState = PlayerController->GetPlayerState<APlayerStateBase>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
    
			return  MainHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UMainAbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    	{
    		if (AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD()))
    		{
    			APlayerStateBase* PlayerState = PlayerController->GetPlayerState<APlayerStateBase>();
    			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
    			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
    			const FWidgetControllerParams AttributeControllerParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
    
    			return  MainHUD->GetAttributeMenuWidgetController(AttributeControllerParams);
    		}
    	}
    	return nullptr;
}


void UMainAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();

	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttribute, Level, PrimaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassDefaultInfo.SecondaryAttribute, Level, SecondaryAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalAttributesContextHandle = AbilitySystemComponent->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	FGameplayEffectSpecHandle VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassDefaultInfo.VitalAttribute, Level, VitalAttributesContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UMainAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass)
{

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return;
	}
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
	
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AbilitySystemComponent->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UMainAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ABaseGameMode* GameModeBase = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));

	if (GameModeBase == nullptr)
	{
		return nullptr;
	} 

	return GameModeBase->CharacterClassInfo;
}

bool UMainAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->IsBlockedHit();
	}
	return false;
}

bool UMainAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FMainGameplayEffectContext* MainEffectContext = static_cast<const FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->IsCriticalHit();
	}
	return false;
}

void UMainAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool InIsBlocked)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->SetIsBlockedHit(InIsBlocked);
	}
}

void UMainAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool InIsCritical)
{
	if (FMainGameplayEffectContext* MainEffectContext = static_cast<FMainGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return MainEffectContext->SetIsCriticalHit(InIsCritical);
	}
}

void UMainAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UMainAbilitySystemLibrary::IsNotFriendly(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}
