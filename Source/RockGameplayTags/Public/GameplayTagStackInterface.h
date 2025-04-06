// Copyright Broken Rock Studios LLC. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagStack.h"
#include "UObject/Interface.h"
#include "GameplayTagStackInterface.generated.h"

/* Interface for assets which contain gameplay tag stacks */
UINTERFACE(BlueprintType, MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UGameplayTagStackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Similiar to the IGameplayTagAssetInterface, but for stacks of gameplay tags.
 */
class ROCKGAMEPLAYTAGS_API IGameplayTagStackInterface
{
	GENERATED_BODY()
public:
	/**
	 * Get any owned gameplay tag stack on the asset
	 * 
	 * @param OutTagContainer	[OUT] tag stack on the asset
	 */
	virtual void GetOwnedGameplayTags(FGameplayTagStackContainer& OutTagContainer) const = 0;

	/**
	 * Check if the asset has a gameplay tag that matches against the specified tag (expands to include parents of asset tags)
	 * 
	 * @param TagToCheck	Tag to check for a match
	 * 
	 * @return True if the asset has a gameplay tag that matches, false if not
	 */
	UFUNCTION(BlueprintCallable, Category=GameplayTags)
	virtual bool HasMatchingGameplayTagStack(FGameplayTag TagToCheck) const;

	/**
	 * Check if the asset has a gameplay tag that matches against the specified tag (expands to include parents of asset tags)
	 * 
	 * @param TagToCheck	Tag to check for a match
	 * @param OutCount		[OUT] number of stacks of the tag
	 * 
	 * @return True if the asset has a gameplay tag that matches, false if not
	 */
	UFUNCTION(BlueprintCallable, Category=GameplayTags)
	virtual bool GetMatchingGameplayTagStack(FGameplayTag TagToCheck, int32& OutCount) const;

	/**
	 * Get the number of stacks of a gameplay tag on the asset
	 * 
	 * @param TagToCheck	Tag to check for a match
	 * 
	 * @return Number of stacks of the tag on the asset. Will return 0 if tag is not present, but the tag might still exist with a stack of 0
	 */
	UFUNCTION(BlueprintCallable, Category=GameplayTags)
	virtual int32 GetGameplayStackCount(const FGameplayTag& TagToCheck) const;
};
