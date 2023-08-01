// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelData.h"
#include "Channels/MovieSceneChannelTraits.h"
#include "MovieSceneClipboard.h"
#include "MovieSceneCustomChannel.generated.h"

USTRUCT()
struct FMyStruct
{
	GENERATED_BODY()

	FMyStruct()
		: Value(0)
	{
		
	}

	friend bool operator==(const FMyStruct& A, const FMyStruct& B)
	{
		return A.Value == B.Value;
	}

	friend bool operator!=(const FMyStruct& A, const FMyStruct& B)
	{
		return A.Value == B.Value;
	}

public:
	UPROPERTY(EditAnywhere)
	TMap<int32, FSoftObjectPath> ObjectPaths;

	UPROPERTY(EditAnywhere)
	int32 Value;
};

/**
 * 
 */
USTRUCT()
struct TRACKPLUGIN_API FMovieSceneCustomChannel : public FMovieSceneChannel
{
	GENERATED_BODY()

	FMovieSceneCustomChannel()
		: bHasDefaultValue(false)
	{
	}

	/**
	 * Serialize this type from another
	 */
	bool SerializeFromMismatchedTag(const FPropertyTag& Tag, FStructuredArchive::FSlot Slot);

	/**
	 * Access a mutable interface for this channel's data
	 *
	 * @return An object that is able to manipulate this channel's data
	 */
	FORCEINLINE TMovieSceneChannelData<FMyStruct> GetData()
	{
		return TMovieSceneChannelData<FMyStruct>(&Times, &Values, &KeyHandles);
	}

	/**
	 * Access a constant interface for this channel's data
	 *
	 * @return An object that is able to interrogate this channel's data
	 */
	FORCEINLINE TMovieSceneChannelData<const FMyStruct> GetData() const
	{
		return TMovieSceneChannelData<const FMyStruct>(&Times, &Values);
	}

	/**
	 * Const access to this channel's times
	 */
	FORCEINLINE TArrayView<const FFrameNumber> GetTimes() const
	{
		return Times;
	}

	/**
	 * Const access to this channel's values
	 */
	FORCEINLINE TArrayView<const FMyStruct> GetValues() const
	{
		return Values;
	}

	/**
	 * Check whether this channel has any data
	 */
	FORCEINLINE bool HasAnyData() const
	{
		return Times.Num() != 0 || bHasDefaultValue == true;
	}

	/**
	 * Evaluate this channel
	 *
	 * @param InTime     The time to evaluate at
	 * @param OutValue   A value to receive the result
	 * @return true if the channel was evaluated successfully, false otherwise
	 */
	bool Evaluate(FFrameTime InTime, FMyStruct& OutValue) const;

	/**
	 * Set the channel's times and values to the requested values
	 */
	FORCEINLINE void Set(TArray<FFrameNumber> InTimes, TArray<FMyStruct> InValues)
	{
		check(InTimes.Num() == InValues.Num());

		Times = MoveTemp(InTimes);
		Values = MoveTemp(InValues);

		KeyHandles.Reset();
		for (int32 Index = 0; Index < Times.Num(); ++Index)
		{
			KeyHandles.AllocateHandle(Index);
		}
	}

	/**
	 * Add keys with these times to channel. The number of elements in both arrays much match or nothing is added.
	 * @param InTimes Times to add
	 * @param InValues Values to add
	 */
	FORCEINLINE void AddKeys(const TArray<FFrameNumber>& InTimes, const TArray<FMyStruct>& InValues)
	{
		check(InTimes.Num() == InValues.Num());
		int32 Index = Times.Num();
		Times.Append(InTimes);
		Values.Append(InValues);
		for (; Index < Times.Num(); ++Index)
		{
			KeyHandles.AllocateHandle(Index);
		}
	}

public:
	// ~ FMovieSceneChannel Interface
	virtual void GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes,
	                     TArray<FKeyHandle>* OutKeyHandles) override;
	virtual void GetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<FFrameNumber> OutKeyTimes) override;
	virtual void
	SetKeyTimes(TArrayView<const FKeyHandle> InHandles, TArrayView<const FFrameNumber> InKeyTimes) override;
	virtual void DuplicateKeys(TArrayView<const FKeyHandle> InHandles, TArrayView<FKeyHandle> OutNewHandles) override;
	virtual void DeleteKeys(TArrayView<const FKeyHandle> InHandles) override;
	virtual void DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore) override;
	virtual void ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate) override;
	virtual TRange<FFrameNumber> ComputeEffectiveRange() const override;
	virtual int32 GetNumKeys() const override;
	virtual void Reset() override;
	virtual void Offset(FFrameNumber DeltaPosition) override;
	virtual void Optimize(const FKeyDataOptimizationParams& InParameters) override;
	virtual void ClearDefault() override;

public:
	/**
	 * Set this channel's default value that should be used when no keys are present
	 *
	 * @param InDefaultValue The desired default value
	 */
	FORCEINLINE void SetDefault(FMyStruct InDefaultValue)
	{
		bHasDefaultValue = true;
		DefaultValue = InDefaultValue;
	}

	/**
	 * Get this channel's default value that will be used when no keys are present
	 *
	 * @return (Optional) The channel's default value
	 */
	FORCEINLINE TOptional<FMyStruct> GetDefault() const
	{
		return bHasDefaultValue ? TOptional<FMyStruct>(DefaultValue) : TOptional<FMyStruct>();
	}

	/**
	 * Remove this channel's default value causing the channel to have no effect where no keys are present
	 */
	FORCEINLINE void RemoveDefault()
	{
		bHasDefaultValue = false;
	}

private:
	UPROPERTY(meta=(KeyTimes))
	TArray<FFrameNumber> Times;

	UPROPERTY()
	FMyStruct DefaultValue;

	UPROPERTY()
	bool bHasDefaultValue;

	UPROPERTY(meta=(KeyValues))
	TArray<FMyStruct> Values;

	/** This needs to be a UPROPERTY so it gets saved into editor transactions but transient so it doesn't get saved into assets. */
	UPROPERTY(Transient)
	FMovieSceneKeyHandleMap KeyHandles;
};

namespace MovieSceneClipboard
{
	template <>
	inline FName GetKeyTypeName<FMyStruct>()
	{
		return "FNiagaraEmitterSectionKey";
	}
}

template <>
struct TStructOpsTypeTraits<FMovieSceneCustomChannel> : public TStructOpsTypeTraitsBase2<FMovieSceneCustomChannel>
{
	enum { WithStructuredSerializeFromMismatchedTag = true };
};

template <>
struct TMovieSceneChannelTraits<FMovieSceneCustomChannel> : TMovieSceneChannelTraitsBase<FMovieSceneCustomChannel>
{
#if WITH_EDITORONLY_DATA
	typedef TMovieSceneExternalValue<FMyStruct> ExtendedEditorDataType;
#endif
};
