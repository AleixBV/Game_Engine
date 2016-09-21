#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	for (p2List_item<Primitive*>* tmp = primitives.getFirst(); tmp != NULL; tmp = tmp->next)
	{
		delete tmp->data;
	}
	primitives.clear();

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	_Plane floor(0, 1, 0, 0);
	floor.axis = true;
	floor.color.Set(255, 255, 255);
	floor.Render();

	for (p2List_item<Primitive*>* tmp = primitives.getFirst(); tmp != NULL; tmp = tmp->next)
	{
		tmp->data->Render();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

void ModuleSceneIntro::CreateCube(const vec3& position, const vec3& size, float angle, const vec3& rotAxis)
{
	_Cube* c = new _Cube();
	c->size.Set(size.x, size.y, size.z);
	c->SetPos(position.x, position.y, position.z);
	if (angle != 0.0f)
		c->SetRotation(angle, rotAxis);

	else
	{
		primitives.add(c);
		App->physics->AddBody(*c, 0);
	}

}

void ModuleSceneIntro::CreateSphere(const vec3& position, float radius)
{
	_Sphere* c = new _Sphere();
	c->radius = radius;
	c->SetPos(position.x, position.y, position.z);

	App->physics->AddBody(*c, 0);

	primitives.add(c);
}