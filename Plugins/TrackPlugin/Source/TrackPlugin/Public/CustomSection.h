// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovieSceneCustomChannel.h"
#include "MovieSceneSection.h"
#include "Channels/MovieSceneFloatChannel.h"
#include "Channels/MovieSceneIntegerChannel.h"
#include "Channels/MovieSceneObjectPathChannel.h"
#include "Sections/MovieSceneActorReferenceSection.h"
#include "CustomSection.generated.h"

USTRUCT(BlueprintType)
struct FCustomTrackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomData")
	FString MapName;
};

/**
 * 
 */
UCLASS()
class TRACKPLUGIN_API UCustomSection : public UMovieSceneSection
{
	GENERATED_BODY()

	UCustomSection();

public:
	UPROPERTY()
	FMovieSceneCustomChannel CustomChannel;
};
