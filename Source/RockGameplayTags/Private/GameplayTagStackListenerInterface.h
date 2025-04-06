// Copyright Broken Rock Studios LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "GameplayTagStackListenerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGameplayTagStackListenerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROCKGAMEPLAYTAGS_API IGameplayTagStackListenerInterface
{
	GENERATED_BODY()
public:
    /**
     * Called when a tag stack changes within a container that this object is listening to.
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GameplayTagStacks")
    void OnTagStackChanged_Internal(const FGameplayTag& Tag, int32 NewCount, int32 OldCount);
};
