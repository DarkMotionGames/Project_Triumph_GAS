// © 2024 DMG. All rights reserved.


#include "UI/Widget/UserWidgetBase.h"

void UUserWidgetBase::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}