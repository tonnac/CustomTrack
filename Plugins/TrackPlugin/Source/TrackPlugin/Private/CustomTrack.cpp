// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTrack.h"

#include "CustomSection.h"

FText UCustomTrack::GetDisplayName() const
{
	static FText TrackName = FText::FromString(FString(TEXT("Custom Data Track")));
	return TrackName;
}


FName UCustomTrack::GetTrackName() const
{
	return UCustomTrack::GetDataTrackName();
}


const FName UCustomTrack::GetDataTrackName()
{
	static FName TheName = FName(*FString(TEXT("CustomDataTrack")));
	return TheName;
}


bool UCustomTrack::IsEmpty() const
{
	return Sections.Num() == 0;
}


UMovieSceneSection* UCustomTrack::CreateNewSection()
{
	UMovieSceneSection* Section = NewObject<UCustomSection>(this, NAME_None, RF_Transactional);
	Sections.Add(Section);
	return Section;
}


void UCustomTrack::AddSection(UMovieSceneSection& Section)
{
	Sections.Add(&Section);
}


void UCustomTrack::RemoveSection(UMovieSceneSection& Section)
{
	Sections.Remove(&Section);
}


const TArray<UMovieSceneSection*>& UCustomTrack::GetAllSections() const 
{
	return Sections;
}