#ifndef __ModuleGeometryLoader_H__
#define __ModuleGeometryLoader_H__

#include "Module.h"
#include "Application.h"

class ModuleGeometryLoader : public Module
{
public:

	//Constructor
	ModuleGeometryLoader(Application* app, bool start_enabled = true);
	//Destructor
	~ModuleGeometryLoader();

	bool Init();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
};

#endif