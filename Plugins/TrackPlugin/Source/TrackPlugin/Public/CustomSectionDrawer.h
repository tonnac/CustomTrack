// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomSection.h"
#include "ISequencerSection.h"
#include "SequencerSectionPainter.h"

/**
 * 
 */
class TRACKPLUGIN_API FCustomSectionDrawer : public ISequencerSection, public TSharedFromThis<FCustomSectionDrawer>
{
public:
	FCustomSectionDrawer(UMovieSceneSection& InSection, TWeakPtr<ISequencer> InSequencer)
	{
		CustomDataSection = Cast<UCustomSection>(&InSection);
	}

	virtual int32 OnPaintSection(FSequencerSectionPainter& InPainter) const override
	{
		return InPainter.PaintSectionBackground();
	}

	virtual UMovieSceneSection* GetSectionObject() override
	{
		return CustomDataSection;
	}

	virtual FText GetSectionTitle() const override
	{
		return FText::FromString(FString(TEXT("Custom Data Section")));
	}

	virtual float GetSectionHeight() const override
	{
		return 40.0f;
	}

	UCustomSection* CustomDataSection;
};
