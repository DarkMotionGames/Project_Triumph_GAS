// © 2024 DMG. All rights reserved.


#include "Player/PlayerStateBase.h"

#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/AttributeSetBase.h"
#include "Net/UnrealNetwork.h"

APlayerStateBase:: APlayerStateBase()
{
	GameAbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>("AbilitySystemComponent");
	GameAbilitySystemComponent->SetIsReplicated(true);
	GameAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	GameAttributeSet = CreateDefaultSubobject<UAttributeSetBase>("AttributeSet");

	SetNetUpdateFrequency(100.f);
}

void APlayerStateBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(APlayerStateBase, Level, COND_None, REPNOTIFY_Always);
}

UAbilitySystemComponent* APlayerStateBase::GetAbilitySystemComponent() const
{
	return GameAbilitySystemComponent;
}

void APlayerStateBase::OnRep_Level(int32 OldLevel)
{
	// OnLevelChangedDelegate.Broadcast(Level, true);
}
