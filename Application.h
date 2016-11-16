#ifndef __Application_H__
#define __Application_H__

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGeometryLoader.h"
#include "ModuleEditor.h"
#include "ModuleFileSystem.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleGeometryLoader* geometry_loader;
	ModuleEditor* editor;
	ModuleFileSystem* file_system;

private:

	const char* title;
	const char* organization;
	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;
	int		max_fps;
	Timer	fps_timer;
	float	frames;
	float	max_ms_per_frame;
	float	ms_in_last_frame;

public:

	Application();
	~Application();

	bool Awake();
	bool Init();
	update_status Update();
	bool CleanUp();


	void SetTitle(const char* title);
	const char* GetTitle() const;
	void SetOrganization(const char* title);
	const char* GetOrganization() const;
	int GetMaxFps();
	void SetMaxFps(int x);
	void OpenLink(char* path);
	void DebugDraw();

private:

	pugi::xml_node LoadConfig(pugi::xml_document& config_file) const;
	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
#endif