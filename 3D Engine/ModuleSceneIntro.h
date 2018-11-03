#ifndef MODULESCENEINTRO_H
#define MODULESCENEINTRO_H


#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();
	GameObject* AddGameObject(GameObject* parent);

	// Variables
	bool grid_enabled = true;
	bool axis_enabled = false;
	
	// Utility
	bool IsRootGO(const GameObject* go) const;

	GameObject* root_gameObjects =  nullptr;
};

#endif // !MODULESCENEINTRO_H
