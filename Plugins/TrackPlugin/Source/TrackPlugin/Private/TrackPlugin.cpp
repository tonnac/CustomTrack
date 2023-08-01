// Copyright Epic Games, Inc. All Rights Reserved.

#include "TrackPlugin.h"

#include "CustomDataTrackEditor.h"
#include "ISequencerModule.h"
#include "MovieSceneCustomChannel.h"
#include "SequencerChannelInterface.h"

#define LOCTEXT_NAMESPACE "FTrackPluginModule"

void FTrackPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	ISequencerModule& SequencerModule = FModuleManager::LoadModuleChecked<ISequencerModule>("Sequencer");
	SequencerModule.RegisterTrackEditor(FOnCreateTrackEditor::CreateStatic(&FCustomDataTrackEditor::CreateTrackEditor));
	SequencerModule.RegisterChannelInterface<FMovieSceneCustomChannel>(
		MakeUnique<TSequencerChannelInterface<FMovieSceneCustomChannel>>());
}

void FTrackPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTrackPluginModule, TrackPlugin)