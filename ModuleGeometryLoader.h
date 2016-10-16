#ifndef __ModuleGeometryLoader_H__
#define __ModuleGeometryLoader_H__

#include "Module.h"
#include "Mesh.h"
#include <vector>

class ModuleGeometryLoader : public Module
{
public:

	//Constructor
	ModuleGeometryLoader(Application* app, bool start_enabled = true);
	//Destructor
	~ModuleGeometryLoader();

	bool Init();
	bool CleanUp();

	bool LoadGeometryFromFile(const char* path);

	std::vector<Mesh> meshes;
};

#endif