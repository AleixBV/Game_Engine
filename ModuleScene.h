#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "GameObject.h"
#include "Mesh.h"
#include <vector>

#define MAX_SNAKE 2
#define DIAMETER_WORLD 2500

struct PhysBody3D;
struct PhysMotor3D;

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	GameObject* CreateNewGameObject(GameObject* parent = nullptr, const char* name = "unnamed");

	GameObject* root = nullptr;

private:
	void CreateSphere(const vec3& position, float radius);
	void CreateCube(const vec3& position, const vec3& size, float angle = 0.0f, const vec3& rotAxis = vec3(1, 1, 1));

	p2List<Primitive*> primitives;
};