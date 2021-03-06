#ifndef __ModuleGeometryLoader_H__
#define __ModuleGeometryLoader_H__

#include "Module.h"
#include "GameObject.h"
#include <vector>

struct aiScene;
struct aiNode;

class ModuleGeometryLoader : public Module
{
public:

	//Constructor
	ModuleGeometryLoader(Application* app, bool start_enabled = true);
	//Destructor
	~ModuleGeometryLoader();

	bool Init();
	bool CleanUp();

	GameObject* RecursiveLoadGeometryFromFile(const aiScene* scene, const aiNode* node, GameObject* parent = nullptr);
	bool LoadGeometryFromFile(const char* path, GameObject* root);
};

#endif