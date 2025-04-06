// Copyright Epic Games, Inc. All Rights Reserved.
// Modified by Broken Rock Studios
// See the LICENSE file for details.

#include "GameplayTagStack.h"

#include "GameplayTagStackListenerInterface.h"
#include "UObject/Stack.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayTagStack)

//////////////////////////////////////////////////////////////////////
// FGameplayTagStack

FString FGameplayTagStack::ToString() const
{
	return FString::Printf(TEXT("%sx%d"), *Tag.ToString(), StackCount);
}

FString FGameplayTagStack::GetDebugString() const
{
	return FString::Printf(TEXT("%sx%d"), *Tag.ToString(), StackCount);
}

int32 FGameplayTagStack::GetStackCount() const
{
	return StackCount;
}

const FGameplayTag& FGameplayTagStack::GetTag() const
{
	return Tag;
}

//////////////////////////////////////////////////////////////////////
// FGameplayTagStackContainer

FGameplayTagStackContainer::FGameplayTagStackContainer()
	: Stacks()
{
#if WITH_PUSH_MODEL
	// OwningObject = nullptr;
#endif
}

void FGameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount, bool bKeepZeroStacks)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to AddStack"), ELogVerbosity::Warning);
		return;
	}

	if (StackCount > 0)
	{
		for (FGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 NewCount = Stack.StackCount + StackCount;
				Stack.StackCount = NewCount;
				TagToCountMap[Tag] = NewCount;
				MarkItemDirty(Stack);
				return;
			}
		}

		FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
		MarkItemDirty(NewStack);
		TagToCountMap.Add(Tag, StackCount);
	}
	else if (StackCount == 0)
	{
		if (bKeepZeroStacks)
		{
			FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, 0);
			MarkItemDirty(NewStack);
			TagToCountMap.Add(Tag, 0);
		}
	}
	else
	{
		// What should we do if StackCount is negative?
	}
}

void FGameplayTagStackContainer::SetStack(FGameplayTag Tag, int32 StackCount, bool bKeepZeroStacks)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to SetStack"), ELogVerbosity::Warning);
		return;
	}

	if (StackCount > 0)
	{
		for (FGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 OldCount = Stack.StackCount;
				Stack.StackCount = StackCount;
				TagToCountMap[Tag] = StackCount;
				MarkItemDirty(Stack);
				return;
			}
		}
	}

	if (bKeepZeroStacks)
	{
		FGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
		MarkItemDirty(NewStack);
		TagToCountMap.Add(Tag, StackCount);
	}
	else
	{
		// Stack wasn't positive and we don't want to keep zero stacks.
		// Remove the stack if it exists.
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			if (It->Tag == Tag)
			{
				It.RemoveCurrent();
				TagToCountMap.Remove(Tag);
				MarkArrayDirty();
				break;
			}
		}
	}
}

bool FGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount, bool bKeepZeroStacks /*= true*/)
{
	if (!Tag.IsValid())
	{
		FFrame::KismetExecutionMessage(TEXT("An invalid tag was passed to RemoveStack"), ELogVerbosity::Warning);
		return false;
	}

	//@TODO: Should we error if you try to remove a stack that doesn't exist or has a smaller count?
	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It; ++It)
		{
			FGameplayTagStack& Stack = *It;
			if (Stack.Tag == Tag)
			{
				if (Stack.StackCount <= StackCount)
				{
					if (bKeepZeroStacks)
					{
						// Keep the tag but set count to zero
						Stack.StackCount = 0;
						TagToCountMap[Tag] = 0;
						MarkItemDirty(Stack);
					}
					else
					{
						It.RemoveCurrent();
						TagToCountMap.Remove(Tag);
						MarkArrayDirty();
					}
				}
				else
				{
					const int32 NewCount = Stack.StackCount - StackCount;
					Stack.StackCount = NewCount;
					TagToCountMap[Tag] = NewCount;
					MarkItemDirty(Stack);
				}
				return true;
			}
		}
	}

	return false;
}

int32 FGameplayTagStackContainer::GetStackCount(FGameplayTag Tag) const
{
	return TagToCountMap.FindRef(Tag);
}

bool FGameplayTagStackContainer::ContainsTag(FGameplayTag Tag) const
{
	return TagToCountMap.Contains(Tag);
}

void FGameplayTagStackContainer::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	IGameplayTagStackListenerInterface* listener = Cast<IGameplayTagStackListenerInterface>(ListenerObject.Get());
	for (int32 Index : RemovedIndices)
	{
		const FGameplayTag Tag = Stacks[Index].Tag;
		TagToCountMap.Remove(Tag);
		if (listener)
		{
			listener->OnTagStackChanged_Internal(Tag, 0, Stacks[Index].StackCount);
		}
	}
}

void FGameplayTagStackContainer::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	IGameplayTagStackListenerInterface* listener = Cast<IGameplayTagStackListenerInterface>(ListenerObject.Get());
	for (int32 Index : AddedIndices)
	{
		const FGameplayTagStack& Stack = Stacks[Index];
		TagToCountMap.Add(Stack.Tag, Stack.StackCount);
		if (listener)
		{
			listener->OnTagStackChanged_Internal(Stack.Tag, Stack.StackCount, 0);
		}
	}
}

void FGameplayTagStackContainer::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	IGameplayTagStackListenerInterface* listener = Cast<IGameplayTagStackListenerInterface>(ListenerObject.Get());
	for (int32 Index : ChangedIndices)
	{
		const FGameplayTagStack& Stack = Stacks[Index];
		int32& StackCount = TagToCountMap.FindOrAdd(Stack.Tag);
		const int32 OldStackCount = StackCount;
		StackCount = Stack.StackCount;
		if (listener)
		{
			listener->OnTagStackChanged_Internal(Stack.Tag, Stack.StackCount, OldStackCount);
		}
	}
}

bool FGameplayTagStackContainer::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FFastArraySerializer::FastArrayDeltaSerialize<FGameplayTagStack, FGameplayTagStackContainer>(Stacks, DeltaParms, *this);
}

void FGameplayTagStackContainer::SetListenerObject(UObject* InListenerObject)
{
	if (InListenerObject && InListenerObject->GetClass()->ImplementsInterface(UGameplayTagStackListenerInterface::StaticClass()))
	{
		// Store a weak pointer for safety against dangling pointers if Listener is destroyed
		ListenerObject = InListenerObject;
	}
	else
	{
		ListenerObject = nullptr;
	}
}
