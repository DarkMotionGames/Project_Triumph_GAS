// © 2024 DMG. All rights reserved.


#include "AbilitySystem/AbilityTasks/TargetDataUnderCrossHair.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UTargetDataUnderCrossHair* UTargetDataUnderCrossHair::CreateTargetDataUnderCrossHair(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderCrossHair* MyObj = NewAbilityTask<UTargetDataUnderCrossHair>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderCrossHair::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendCrossHairData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredicationKey = GetActivationPredictionKey();

		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredicationKey).AddUObject(this, &UTargetDataUnderCrossHair::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredicationKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderCrossHair::SendCrossHairData()
{
	
	if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{

		FScopedPredictionWindow ScopedPredication(AbilitySystemComponent.Get());
		APlayerController* PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();

		FVector Start = PlayerCharacter->FollowCamera->GetComponentLocation();
		FVector End = PlayerCharacter->FollowCamera->GetComponentQuat().Vector();

		FVector EndFar = End * 12000.f;
		FVector TrueEnd = Start + EndFar;

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwnerActor());

		FHitResult HitResults;
		const bool Hit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Start, TrueEnd,
			10.f, UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false, ActorsToIgnore, EDrawDebugTrace::None,
			HitResults, true, FLinearColor::Red,
			FLinearColor::Green, 0.f);
    
		FGameplayAbilityTargetDataHandle DataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
		Data->HitResult = HitResults;
		DataHandle.Add(Data);

		AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
			GetActivationPredictionKey(), DataHandle, FGameplayTag(),
			AbilitySystemComponent->ScopedPredictionKey);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			ValidData.Broadcast(DataHandle);
		}
	}
}

void UTargetDataUnderCrossHair::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
