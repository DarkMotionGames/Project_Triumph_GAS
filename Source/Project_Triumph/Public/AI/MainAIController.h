// © 2024 DMG. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MainAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class PROJECT_TRIUMPH_API AMainAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMainAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;
};
