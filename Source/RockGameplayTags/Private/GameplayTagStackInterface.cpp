// Copyright Broken Rock Studios LLC. All Rights Reserved.


#include "GameplayTagStackInterface.h"


bool IGameplayTagStackInterface::HasMatchingGameplayTagStack(FGameplayTag TagToCheck) const
{
	FGameplayTagStackContainer OutTagContainer;
	GetOwnedGameplayTags(OutTagContainer);
	return OutTagContainer.ContainsTag(TagToCheck);
}

bool IGameplayTagStackInterface::GetMatchingGameplayTagStack(FGameplayTag TagToCheck, int32& OutCount) const
{
	FGameplayTagStackContainer OutTagContainer;
	GetOwnedGameplayTags(OutTagContainer);
	if (OutTagContainer.ContainsTag(TagToCheck))
	{
		OutCount = OutTagContainer.GetStackCount(TagToCheck);
		return true;
	}
	OutCount = 0;
	return false;

}

int32 IGameplayTagStackInterface::GetGameplayStackCount(const FGameplayTag& TagToCheck) const
{
	FGameplayTagStackContainer OutTagContainer;
	GetOwnedGameplayTags(OutTagContainer);
	if (OutTagContainer.ContainsTag(TagToCheck))
	{
		return OutTagContainer.GetStackCount(TagToCheck);
	}
	return 0;
}
