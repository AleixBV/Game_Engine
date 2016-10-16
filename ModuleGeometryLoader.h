#ifndef __ModuleGeometryLoader_H__
#define __ModuleGeometryLoader_H__

#include "Module.h"
#include <vector>

struct Mesh
{
public:
	Mesh();
	~Mesh();

	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;
};

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