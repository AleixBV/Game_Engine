#include "Globals.h"
#include "ModuleGeometryLoader.h"

//Constructor
ModuleGeometryLoader::ModuleGeometryLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

//Destructor
ModuleGeometryLoader::~ModuleGeometryLoader()
{

}

bool ModuleGeometryLoader::Init()
{
	return true;
}

bool ModuleGeometryLoader::CleanUp()
{
	return true;
}

update_status ModuleGeometryLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleGeometryLoader::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return ret;
}

update_status ModuleGeometryLoader::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}