// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UUserWidgetBase;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API AMainHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WidConParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WidConParams);
	void InitOverlay(APlayerController* FPlayerController, APlayerState* FPlayerState, UAbilitySystemComponent* FAbilitySystemComponent, UAttributeSet* FAttributeSet);
	
private:

	UPROPERTY()
	TObjectPtr<UUserWidgetBase> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidgetBase> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
