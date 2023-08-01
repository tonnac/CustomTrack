// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSection.h"

#include "Channels/MovieSceneChannelProxy.h"
#include "UObject/SequencerObjectVersion.h"

struct FCustomChannelEditorData
{
	FCustomChannelEditorData()
	{
		Data[0].SetIdentifiers("Volume", NSLOCTEXT("MovieSceneAudioSection", "SoundVolumeText", "Volume"));
		Data[1].SetIdentifiers("Pitch", NSLOCTEXT("MovieSceneAudioSection", "PitchText", "Pitch"));
		Data[2].SetIdentifiers("AttachActor", NSLOCTEXT("MovieSceneAudioSection", "AttachActorText", "Attach"));
	}

	FMovieSceneChannelMetaData Data[3];
};

UCustomSection::UCustomSection()
{
	EvalOptions.EnableAndSetCompletionMode
	(GetLinkerCustomVersion(FSequencerObjectVersion::GUID) <
	 FSequencerObjectVersion::WhenFinishedDefaultsToRestoreState
		 ? EMovieSceneCompletionMode::KeepState
		 : GetLinkerCustomVersion(FSequencerObjectVersion::GUID) <
		 FSequencerObjectVersion::WhenFinishedDefaultsToProjectDefault
		 ? EMovieSceneCompletionMode::RestoreState
		 : EMovieSceneCompletionMode::ProjectDefault);
	BlendType = EMovieSceneBlendType::Absolute;
	bSupportsInfiniteRange = true;


	SetRange(TRange<FFrameNumber>::All());

	// FMovieSceneChannelProxyData Channels;
	//
	// FCustomChannelEditorData EditorData;
	// Channels.Add(SoundVolume, EditorData.Data[0], TMovieSceneExternalValue<float>());
	// Channels.Add(PitchMultiplier, EditorData.Data[1], TMovieSceneExternalValue<float>());
	// // Channels.Add(AttachActorData, EditorData.Data[2]);
	//
	// ChannelProxy = MakeShared<FMovieSceneChannelProxy>(MoveTemp(Channels));

	ChannelProxy = MakeShared<FMovieSceneChannelProxy>(CustomChannel, FMovieSceneChannelMetaData(), TMovieSceneExternalValue<FMyStruct>::Make());
}
