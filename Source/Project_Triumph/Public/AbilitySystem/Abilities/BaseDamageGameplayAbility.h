// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UBaseDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Damage")
	void CauseDamage(AActor* TargetActor);

	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
