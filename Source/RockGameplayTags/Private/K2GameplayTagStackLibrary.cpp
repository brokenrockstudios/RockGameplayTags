// Copyright Broken Rock Studios LLC. All Rights Reserved.


#include "K2GameplayTagStackLibrary.h"

#include "GameplayTagContainer.h"
#include "GameplayTagStack.h"


void UK2GameplayTagStackLibrary::AddGameplayTagStack(FGameplayTagStackContainer& TagContainer, FGameplayTag Tag, int32 StackCount)
{
	TagContainer.AddStack(Tag, StackCount);
}

void UK2GameplayTagStackLibrary::RemoveGameplayTagStack(
	FGameplayTagStackContainer& TagContainer, FGameplayTag Tag, int32 StackCount, bool bRemoveOnZero)
{
	TagContainer.RemoveStack(Tag, StackCount, bRemoveOnZero);
}

void UK2GameplayTagStackLibrary::SetGameplayTagStack(FGameplayTagStackContainer& TagContainer, FGameplayTag Tag, int32 StackCount, bool bRemoveOnZero)
{
	TagContainer.SetStack(Tag, StackCount, bRemoveOnZero);
}

int32 UK2GameplayTagStackLibrary::GetGameplayTagStackCount(const FGameplayTagStackContainer& TagContainer, FGameplayTag Tag)
{
	return TagContainer.GetStackCount(Tag);
}
