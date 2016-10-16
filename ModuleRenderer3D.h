#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Mesh.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool debug_draw;

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void DrawAllMeshes();
	void DrawMesh(Mesh* mesh);
	void ModuleRenderer3D::BeginDebugDraw();
	void ModuleRenderer3D::EndDebugDraw();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};