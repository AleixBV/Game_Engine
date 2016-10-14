#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGeometryLoader.h"
#include "ModuleEditor.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleGeometryLoader* geometry_loader;
	ModuleEditor* editor;

private:

	Timer	ms_timer;
	float	dt;
	p2List<Module*> list_modules;
	int		max_fps;
	Timer	fps_timer;
	float	frames;
	float	max_ms_per_frame;
	float	ms_in_last_frame;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	int GetMaxFps();
	void SetMaxFps(int x);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};