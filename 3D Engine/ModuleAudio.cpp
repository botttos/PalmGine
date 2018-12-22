#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "CompAudioSource.h"

ModuleAudio::ModuleAudio(bool start_enabled) : Module(start_enabled)
{
	
}

ModuleAudio::~ModuleAudio()
{}

bool ModuleAudio::Start()
{
	// Init wwise and audio banks
	WwiseT::InitSoundEngine();
	WwiseT::LoadBank("Assignment3.bnk");
	return true;
}

update_status ModuleAudio::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	WwiseT::ProcessAudio();
	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp() {
	
	audio_sources.clear();

	return true;
}

WwiseT::AudioSource * ModuleAudio::CreateSoundEmitter(const char * name)
{
	WwiseT::AudioSource* ret = WwiseT::CreateAudSource(name);
	event_list.push_back(ret);
	return ret;
}

uint ModuleAudio::GetListenerID() const
{
	return listener->GetID();
}

void ModuleAudio::Play() const
{
	std::list<CompAudioSource*>::const_iterator iterator;
	for (iterator = App->audio->audio_sources.begin(); iterator != App->audio->audio_sources.end(); ++iterator) {
		if (iterator._Ptr->_Myval->GetPlayOnAwake() == true) {
			iterator._Ptr->_Myval->PlayAudio();
		}
	}
}

void ModuleAudio::Stop() const
{
	std::list<CompAudioSource*>::const_iterator iterator;
	for (iterator = App->audio->audio_sources.begin(); iterator != App->audio->audio_sources.end(); ++iterator) {
		iterator._Ptr->_Myval->StopAudio();
	}
}

void ModuleAudio::Pause() const
{
	std::list<CompAudioSource*>::const_iterator iterator;
	for (iterator = App->audio->audio_sources.begin(); iterator != App->audio->audio_sources.end(); ++iterator) {
		iterator._Ptr->_Myval->PauseAudio();
	}
}

void ModuleAudio::Resume() const
{
	std::list<CompAudioSource*>::const_iterator iterator;
	for (iterator = App->audio->audio_sources.begin(); iterator != App->audio->audio_sources.end(); ++iterator) {
		iterator._Ptr->_Myval->ResumeAudio();
	}
}
