// Copyright Broken Rock Studios LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagStack.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "K2GameplayTagStackLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ROCKGAMEPLAYTAGS_API UK2GameplayTagStackLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "GameplayTags")
	static void AddGameplayTagStack(FGameplayTagStackContainer& TagContainer, FGameplayTag Tag, int32 StackCount);

	UFUNCTION(BlueprintCallable, Category = "GameplayTags")
	static void RemoveGameplayTagStack(FGameplayTagStackContainer& TagContainer, FGameplayTag Tag, int32 StackCount, bool bRemoveOnZero = true);
	UFUNCTION(BlueprintCallable, Category = "GameplayTags")
	static void SetGameplayTagStack(FGameplayTagStackContainer& TagContainer, FGameplayTag Tag, int32 StackCount, bool bRemoveOnZero = true);
	UFUNCTION(BlueprintCallable, Category = "GameplayTags")
	static int32 GetGameplayTagStackCount(const FGameplayTagStackContainer& TagContainer, FGameplayTag Tag);

	
};
