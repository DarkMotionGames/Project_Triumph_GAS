// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "PlayerControllerBase.generated.h"

class UDamageTextComponent;
class UInputConfig;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class UAbilitySystemComponentBase;


DECLARE_MULTICAST_DELEGATE(FTabKeyPressedSignature);

struct FInputActionValue;
/**
 * 
 */
UCLASS()	
class PROJECT_TRIUMPH_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()


public:
	APlayerControllerBase();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);
	
	FTabKeyPressedSignature TabKeyPressedDelegate;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> TabAction;

	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	void Jump();
	void StopJump();
	void OpenTabMenu();;

	void CursorTrace();
	
	TScriptInterface<IEnemyInterface> LastHitActor;
	TScriptInterface<IEnemyInterface> CurrentHitActor;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputConfig> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponentBase> AbilitySystemComponentBase;
	
	UAbilitySystemComponentBase* GetAbilitySystemComponentBase();

	UPROPERTY(EditDefaultsOnly, Category="Widget")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	void AttachDamageTextToCharacter(USceneComponent* DamageText, const ACharacter* TargetCharacter, FName SocketName);
};
