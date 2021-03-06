#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "ComponentMesh.h"
#include "Color.h"
#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"

#define MAX_LIGHTS 8

//class ComponentMesh;

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

private:
	void DrawGameObjects(const GameObject* game_object);
	void DrawMesh(const ComponentMesh* mesh, int material_id = -1, GameObject::WireframeTypeDraw type_draw = GameObject::WIREFRAME_NORMAL_DRAW);
	void DrawWireframeBox(const float3* corners, Color color);
	void BeginDebugDraw();
	void EndDebugDraw();

	unsigned int BlendType(ComponentMesh::BlendType blendtype);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};