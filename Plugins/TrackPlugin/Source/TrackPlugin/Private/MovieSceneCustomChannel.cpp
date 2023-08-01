// Fill out your copyright notice in the Description page of Project Settings.


#include "MovieSceneCustomChannel.h"

#include "MovieSceneFrameMigration.h"
#include "Channels/MovieSceneChannelTraits.h"

bool FMovieSceneCustomChannel::SerializeFromMismatchedTag(const FPropertyTag& Tag, FStructuredArchive::FSlot Slot)
{
	static const FName IntegralCurveName("IntegralCurve");
	if (Tag.Type == NAME_StructProperty && Tag.StructName == IntegralCurveName)
	{
		FIntegralCurve IntegralCurve;
		FIntegralCurve::StaticStruct()->SerializeItem(Slot, &IntegralCurve, nullptr);

		// if (IntegralCurve.GetDefaultValue() != MAX_int32)
		// {
		// 	bHasDefaultValue = true;
		// 	DefaultValue = IntegralCurve.GetDefaultValue();
		// }

		Times.Reserve(IntegralCurve.GetNumKeys());
		Values.Reserve(IntegralCurve.GetNumKeys());

		FFrameRate LegacyFrameRate = GetLegacyConversionFrameRate();

		// int32 Index = 0;
		// for (auto It = IntegralCurve.GetKeyIterator(); It; ++It)
		// {
		// 	FFrameNumber KeyTime = UpgradeLegacyMovieSceneTime(nullptr, LegacyFrameRate, It->Time);
		//
		// 	int32 Val = It->Value;
		// 	ConvertInsertAndSort<int32>(Index++, KeyTime, Val, Times, Values);
		// }
		return true;
	}

	return false;
}

bool FMovieSceneCustomChannel::Evaluate(FFrameTime InTime, FMyStruct& OutValue) const
{
	if (Times.Num())
	{
		const int32 Index = FMath::Max(0, Algo::UpperBound(Times, InTime.FrameNumber) - 1);
		OutValue = Values[Index];
		return true;
	}
	else if (bHasDefaultValue)
	{
		OutValue = DefaultValue;
		return true;
	}

	return false;
}

void FMovieSceneCustomChannel::GetKeys(const TRange<FFrameNumber>& WithinRange, TArray<FFrameNumber>* OutKeyTimes,
                                        TArray<FKeyHandle>* OutKeyHandles)
{
	GetData().GetKeys(WithinRange, OutKeyTimes, OutKeyHandles);
}

void FMovieSceneCustomChannel::GetKeyTimes(TArrayView<const FKeyHandle> InHandles,
                                            TArrayView<FFrameNumber> OutKeyTimes)
{
	GetData().GetKeyTimes(InHandles, OutKeyTimes);
}

void FMovieSceneCustomChannel::SetKeyTimes(TArrayView<const FKeyHandle> InHandles,
                                            TArrayView<const FFrameNumber> InKeyTimes)
{
	GetData().SetKeyTimes(InHandles, InKeyTimes);
}

void FMovieSceneCustomChannel::DuplicateKeys(TArrayView<const FKeyHandle> InHandles,
                                              TArrayView<FKeyHandle> OutNewHandles)
{
	GetData().DuplicateKeys(InHandles, OutNewHandles);
}

void FMovieSceneCustomChannel::DeleteKeys(TArrayView<const FKeyHandle> InHandles)
{
	GetData().DeleteKeys(InHandles);
}

void FMovieSceneCustomChannel::DeleteKeysFrom(FFrameNumber InTime, bool bDeleteKeysBefore)
{
	// Insert a key at the current time to maintain evaluation
	if (GetData().GetTimes().Num() > 0)
	{
		FMyStruct Value;
		if (Evaluate(InTime, Value))
		{
			GetData().UpdateOrAddKey(InTime, Value);
		}
	}

	GetData().DeleteKeysFrom(InTime, bDeleteKeysBefore);
}

void FMovieSceneCustomChannel::ChangeFrameResolution(FFrameRate SourceRate, FFrameRate DestinationRate)
{
	GetData().ChangeFrameResolution(SourceRate, DestinationRate);
}

TRange<FFrameNumber> FMovieSceneCustomChannel::ComputeEffectiveRange() const
{
	return GetData().GetTotalRange();
}

int32 FMovieSceneCustomChannel::GetNumKeys() const
{
	return Times.Num();
}

void FMovieSceneCustomChannel::Reset()
{
	Times.Reset();
	Values.Reset();
	KeyHandles.Reset();
	bHasDefaultValue = false;
}

void FMovieSceneCustomChannel::Optimize(const FKeyDataOptimizationParams& InParameters)
{
	UE::MovieScene::Optimize(this, InParameters);
}

void FMovieSceneCustomChannel::Offset(FFrameNumber DeltaPosition)
{
	GetData().Offset(DeltaPosition);
}

void FMovieSceneCustomChannel::ClearDefault()
{
	bHasDefaultValue = false;
}
