// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneTrack.h"
#include "CustomTrack.generated.h"

/**
 * 
 */
UCLASS()
class TRACKPLUGIN_API UCustomTrack : public UMovieSceneTrack
{
	GENERATED_BODY()
	
public:

	virtual FText GetDisplayName() const override;
	virtual FName GetTrackName() const override;

	virtual bool IsEmpty() const override;


	//Basic section operations: 
	virtual class UMovieSceneSection* CreateNewSection() override;
	virtual void AddSection(class UMovieSceneSection& Section) override;
	virtual void RemoveSection(UMovieSceneSection& Section) override;


	virtual const TArray<UMovieSceneSection*>& GetAllSections() const override;


	static const FName GetDataTrackName();

	UPROPERTY()
	TArray<UMovieSceneSection*> Sections;
};
