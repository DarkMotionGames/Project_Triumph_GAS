// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "BaseProjectileSpell.generated.h"

class AProjectileBase;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UBaseProjectileSpell : public UBaseDamageGameplayAbility, public ICombatInterface
{
	GENERATED_BODY()


protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
};
