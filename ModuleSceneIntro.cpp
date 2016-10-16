#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModuleGeometryLoader.h"
#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")


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

	glewInit();

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
	for (p2List_item<Primitive*>* tmp = primitives.getFirst(); tmp != NULL; tmp = tmp->next)
	{
		tmp->data->Render();
	}


	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		App->geometry_loader->LoadGeometryFromFile("Game/Assets/warrior.fbx");
	}

	/*
	//DIRECT MODE!
	
	float v0[3] = { 1.0f, 1.0f, 1.0f };
	float v1[3] = { 0.0f, 1.0f, 1.0f };
	float v2[3] = { 0.0f, 0.0f, 1.0f };
	float v3[3] = { 1.0f, 0.0f, 1.0f };
	float v4[3] = { 1.0f, 0.0f, 0.0f };
	float v5[3] = { 1.0f, 1.0f, 0.0f };
	float v6[3] = { 0.0f, 1.0f, 0.0f };
	float v7[3] = { 0.0f, 0.0f, 0.0f };

	glBegin(GL_TRIANGLES);  // draw a cube with 12 triangles

							// front face =================
	glVertex3fv(v0);    // v0-v1-v2
	glVertex3fv(v1);
	glVertex3fv(v2);
	
	glVertex3fv(v2);    // v2-v3-v0
	glVertex3fv(v3);
	glVertex3fv(v0);
	
	// right face =================
	glVertex3fv(v0);    // v0-v3-v4
	glVertex3fv(v3);
	glVertex3fv(v4);
	
	glVertex3fv(v4);    // v4-v5-v0
	glVertex3fv(v5);
	glVertex3fv(v0);

	// top face ===================
	glVertex3fv(v0);    // v0-v5-v6
	glVertex3fv(v5);
	glVertex3fv(v6);

	glVertex3fv(v6);    // v6-v1-v0
	glVertex3fv(v1);
	glVertex3fv(v0);

	// lower face ===================
	glVertex3fv(v7);    // v7-v4-v3
	glVertex3fv(v4);
	glVertex3fv(v3);

	glVertex3fv(v3);    // v3-v2-v7
	glVertex3fv(v2);
	glVertex3fv(v7);

	// left face ===================
	glVertex3fv(v1);    // v1-v6-v7
	glVertex3fv(v6);
	glVertex3fv(v7);

	glVertex3fv(v7);    // v7-v2-v1
	glVertex3fv(v2);
	glVertex3fv(v1);

	// rear face ===================
	glVertex3fv(v7);    // v7-v6-v5
	glVertex3fv(v6);
	glVertex3fv(v5);

	glVertex3fv(v5);    // v5-v4-v7
	glVertex3fv(v4);
	glVertex3fv(v7);
	
	
		glEnd();*/

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