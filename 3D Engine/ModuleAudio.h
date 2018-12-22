#ifndef MODULEAUDIO_H
#define MODULEAUDIO_H

#include "Module.h"
#include "Globals.h"
#include "WwiseT.h"

class ModuleAudio : public Module
{
public:
	ModuleAudio(bool start_enabled = true);
	~ModuleAudio();
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	// Set listener
	// Create emmiter
	// Delete emmiter
	
	WwiseT::AudioSource* test1 = nullptr;
	WwiseT::AudioSource* test2 = nullptr;
	WwiseT::AudioSource* listener;

private:
	std::list<WwiseT::AudioSource*> event_list;
};

#endif // !MODULEAUDIO_H