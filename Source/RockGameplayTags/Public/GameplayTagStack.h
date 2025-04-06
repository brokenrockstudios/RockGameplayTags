// Copyright Epic Games, Inc. All Rights Reserved.
// Modified by Broken Rock Studios
// See the LICENSE file for details.

#pragma once

#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "GameplayTagStack.generated.h"

struct FGameplayTagStackContainer;
struct FNetDeltaSerializeInfo;

/**
 * Represents one stack of a gameplay tag (tag + count)
 */
USTRUCT(BlueprintType)
struct ROCKGAMEPLAYTAGS_API FGameplayTagStack : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FGameplayTagStack()
	{
	}

	FGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		  , StackCount(InStackCount)
	{
	}

	const FGameplayTag& GetTag() const;
	int32 GetStackCount() const;

	bool IsValid() const
	{
		return Tag.IsValid();
	}

	FString ToString() const;
	FString GetDebugString() const;

	FName GetTagName() const
	{
		return Tag.GetTagName();
	}

	bool MatchesTag(FGameplayTag InTag) const
	{
		return Tag.MatchesTag(InTag);
	}

	bool MatchesTagExact(FGameplayTag InTag) const
	{
		return Tag.MatchesTagExact(InTag);
	}

	bool MatchesTag(FGameplayTag InTag, int32 InCount) const
	{
		return Tag.MatchesTag(InTag) && StackCount >= InCount;
	}

	bool MatchesAny(const FGameplayTagContainer& InContainer) const
	{
		return Tag.MatchesAny(InContainer);
	}

	bool MatchesAnyExact(const FGameplayTagContainer& InContainer) const
	{
		return Tag.MatchesAnyExact(InContainer);
	}

protected:
	UPROPERTY(EditAnywhere, Category = "GameplayTagStack")
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, Category = "GameplayTagStack")
	int32 StackCount = 0;
		
	friend FGameplayTagStackContainer;
};

/** Container of gameplay tag stacks */
USTRUCT(BlueprintType)
struct ROCKGAMEPLAYTAGS_API FGameplayTagStackContainer : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FGameplayTagStackContainer();

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	void AddStack(FGameplayTag Tag, int32 StackCount, bool bKeepZeroStacks = false);

	// Sets a stat tag stack
	void SetStack(FGameplayTag Tag, int32 StackCount, bool bKeepZeroStacks = false);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	// If bKeepZeroStacks is true, the stack will not be removed if it reaches 0
	bool RemoveStack(FGameplayTag Tag, int32 StackCount, bool bKeepZeroStacks = false);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	int32 GetStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	bool ContainsTag(FGameplayTag Tag) const;

	int32* FindStackCount(FGameplayTag Tag);

	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);

	// ~Begin of TArray interface
	auto& operator[](int32 Index) { return Stacks[Index]; }
	const auto& operator[](int32 Index) const { return Stacks[Index]; }
	int32 Num() const { return Stacks.Num(); }

	/**
	 * DO NOT USE DIRECTLY
	 * STL-like iterators to enable range-based for loop support.
	 */
	auto begin() { return Stacks.begin(); }
	auto begin() const { return Stacks.begin(); }
	auto end() { return Stacks.end(); }
	auto end() const { return Stacks.end(); }
	auto rbegin() { return Stacks.rbegin(); }
	auto rbegin() const { return Stacks.rbegin(); }
	auto rend() { return Stacks.rend(); }
	auto rend() const { return Stacks.rend(); }
	// ~End of TArray interface

	void SetListenerObject(UObject* InListenerObject);
private:
	// Replicated list of gameplay tag stacks
	UPROPERTY()
	TArray<FGameplayTagStack> Stacks;

	// Accelerated list of tag stacks for queries
	TMap<FGameplayTag, int32> TagToCountMap;

	UPROPERTY()
	TWeakObjectPtr<UObject> ListenerObject;
};

inline int32* FGameplayTagStackContainer::FindStackCount(FGameplayTag Tag)
{
	return TagToCountMap.Find(Tag);
}

template <>
struct TStructOpsTypeTraits<FGameplayTagStackContainer> : public TStructOpsTypeTraitsBase2<FGameplayTagStackContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
