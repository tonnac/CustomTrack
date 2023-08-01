// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomDataTrackEditor.h"

#include "CustomSectionDrawer.h"
#include "CustomTrack.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "SequencerAwesomness"


FCustomDataTrackEditor::FCustomDataTrackEditor(TSharedRef<ISequencer>InSequencer) : FMovieSceneTrackEditor(InSequencer){}


TSharedRef<ISequencerTrackEditor> FCustomDataTrackEditor::CreateTrackEditor(TSharedRef<ISequencer> InSequencer)
{
	return MakeShareable(new FCustomDataTrackEditor(InSequencer));
}

//Please take a note that on Button click we are creating our Track and its section.
void FCustomDataTrackEditor::BuildAddTrackMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		LOCTEXT("AddMetaDataTrack", "Add Meta Data Track"), //Label
		LOCTEXT("AddMetaDataTrack", "Add Meta Data Track"), //Tooltip
		FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.TextRenderComponent"),
		FUIAction(FExecuteAction::CreateLambda([=]
		{
			UMovieScene* FocusedMovieScene = GetFocusedMovieScene();
			if (IsValid(FocusedMovieScene))
			{
				//Create scoped transaction:
				// const FScopedTransaction(LOCTEXT("FPMCStringsTrackEditor_Transaction", "Add Meta Data Track"));

				FocusedMovieScene->Modify();
				UCustomTrack* NewTrack = FocusedMovieScene->AddMasterTrack<UCustomTrack>();
				FCustomDataTrackEditor::AddNewTrack(NewTrack, FocusedMovieScene);


				//Notify SequencerAboutTransactions:
				GetSequencer()->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
			}
		})));
}


bool FCustomDataTrackEditor::SupportsType(TSubclassOf<UMovieSceneTrack> Type) const
{
	return UCustomTrack::StaticClass() == Type;
}


bool FCustomDataTrackEditor::SupportsSequence(UMovieSceneSequence* InSequence) const
{
	if (!IsValid(InSequence))
	{
		return false;
	}


	static UClass* LevelSequencerClass = FindObject<UClass>(ANY_PACKAGE, TEXT("LevelSequence"), true);
	bool bValidClasses = true;


	bValidClasses &= (LevelSequencerClass != nullptr);
	bValidClasses &= InSequence->GetClass()->IsChildOf(LevelSequencerClass);


	return bValidClasses;
}


FReply FCustomDataTrackEditor::AddNewTrack(UMovieSceneTrack* Track, UMovieScene* FocusedMovieScene)
{
	if (IsValid(FocusedMovieScene))
	{
		UCustomTrack* CustomTrack = Cast<UCustomTrack>(Track);
		if (IsValid(CustomTrack)) 
		{
			// Transactions mark on track
			Track->Modify(); 
			TRange<FFrameNumber> SectionRange = FocusedMovieScene->GetPlaybackRange();
			UMovieSceneSection* NewSection = Track->CreateNewSection();
			UCustomSection* DataSection = Cast<UCustomSection>(NewSection);


 			if (IsValid(DataSection))
 			{
 				// NewSection->SetRange(SectionRange);
				for (TObjectIterator<UWorld> Itr; Itr; ++Itr)
				{
					UWorld* World = *Itr;
					if (World->IsEditorWorld() && !World->IsTemplate())
					{
						FString LevelName = World->GetMapName();
						LevelName.RemoveFromStart(World->StreamingLevelsPrefix);
						// DataSection->CustomTrackData.MapName = LevelName;
					}
				}
 			}
		}
	}


	return FReply::Handled();
}


TSharedRef<ISequencerSection> FCustomDataTrackEditor::MakeSectionInterface(UMovieSceneSection& SectionObject, UMovieSceneTrack& Track, FGuid ObjectBinding)
{
	return MakeShareable(new FCustomSectionDrawer(SectionObject, GetSequencer()));
}


TSharedPtr<SWidget> FCustomDataTrackEditor::BuildOutlinerEditWidget(const FGuid& ObjectBinding, UMovieSceneTrack* Track, const FBuildEditWidgetParams& Params)
{
	return nullptr;
}


#undef LOCTEXT_NAMESPACE