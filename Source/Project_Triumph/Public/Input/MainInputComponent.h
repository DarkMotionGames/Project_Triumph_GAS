// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "InputConfig.h"
#include "MainInputComponent.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API UMainInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object,PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UMainInputComponent::BindAbilityActions(const UInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const FInputActionConfig& ActionConfig : InputConfig -> AbilityInputActions)
	{
		if (ActionConfig.InputAction && ActionConfig.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(ActionConfig.InputAction, ETriggerEvent::Started, Object, PressedFunc, ActionConfig.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(ActionConfig.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, ActionConfig.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(ActionConfig.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, ActionConfig.InputTag);
			}
		}
	}
}
