// © 2024 DMG. All rights reserved.


#include "UI/WidgetController/MainWidgetController.h"

void UMainWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidCoParams)
{
	PlayerController = WidCoParams.PlayerController;
	PlayerState = WidCoParams.PlayerState;
	AbilitySystemComponent = WidCoParams.AbilitySystemComponent;
	AttributeSet = WidCoParams.AttributeSet;
}

void UMainWidgetController::BroadcastInitialValues()
{
}

void UMainWidgetController::BindCallbacksToDependencies()
{
}
