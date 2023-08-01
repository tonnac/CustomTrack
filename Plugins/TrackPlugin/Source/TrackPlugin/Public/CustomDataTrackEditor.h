// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrackEditor.h"

/**
 * 
 */
class TRACKPLUGIN_API FCustomDataTrackEditor : public FMovieSceneTrackEditor
{
public:
	FCustomDataTrackEditor(TSharedRef<ISequencer> InSequencer);
	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor(TSharedRef<ISequencer> InSequencer);

	//Add our track to the add track menu
	virtual void BuildAddTrackMenu(FMenuBuilder& MenuBuilder) override;

	//Draw the track entry itself (as this is an SWidget, it can be customized and additional button or labels can be added
	virtual TSharedPtr<SWidget> BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track,
	                                                    const FBuildEditWidgetParams& Params);


	virtual TSharedRef<ISequencerSection> MakeSectionInterface(UMovieSceneSection& SectionObject,
	                                                           UMovieSceneTrack& Track, FGuid ObjectBinding) override;
	virtual bool SupportsType(TSubclassOf<UMovieSceneTrack> Type) const override;
	virtual bool SupportsSequence(UMovieSceneSequence* InSequence) const override;


	static FReply AddNewTrack(UMovieSceneTrack* track, UMovieScene* focusedMovieScene);
};
