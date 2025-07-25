// © 2024 DMG. All rights reserved.


#include "Actor/GameEffector.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"


AGameEffector::AGameEffector()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AGameEffector::BeginPlay()
{
	Super::BeginPlay();
}

void AGameEffector::ApplyEffectToTarget(AActor* TargetActor, TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClasses)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies)
	{
		return;
	}
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAbilitySystemComponent == nullptr)
	{
		return;
	}
	for (const TSubclassOf<UGameplayEffect> GameplayEffectClass : GameplayEffectClasses)
	{
		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetAbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);	
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
		const bool bIsInfinite =  EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite && InfiniteEffectRemovalPolicy == RemoveOnEndOverlap)
		{
			ActiveEffectHandles.Add(ActiveEffectHandle, TargetAbilitySystemComponent);
		}
		if (!bIsInfinite)
		{
			Destroy();
		}
	}
}

void AGameEffector::OnBeginOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies)
	{
		return;
	}
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClasses);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClasses);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClasses);
	}
}

void AGameEffector::OnEndOverlap(AActor* TargetActor)
{
	if (TargetActor->ActorHasTag(FName("Enemy")) && !bApplyEffectToEnemies)
	{
		return;
	}
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClasses);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClasses);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClasses);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetActorAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetActorAbilitySystemComponent)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (TargetActorAbilitySystemComponent == HandlePair.Value)
			{
				TargetActorAbilitySystemComponent->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
		for (const TSubclassOf<UGameplayEffect> GameplayEffectClass : InfiniteGameplayEffectClasses)
		{
			TargetActorAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GameplayEffectClass, TargetActorAbilitySystemComponent, 1);
		}
	}
}
