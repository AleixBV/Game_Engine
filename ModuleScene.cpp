#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModuleGeometryLoader.h"
#include "ComponentCamera.h"
#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")


ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "scene";
}

ModuleScene::~ModuleScene()
{
	RELEASE(root);
}

// Load assets
bool ModuleScene::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	glewInit();

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	root = new GameObject(nullptr, "root");

	main_camera = new GameObject(root, "main_camera");
	ComponentCamera* camera = new ComponentCamera();
	main_camera->components.push_back(camera);

	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Intro scene");

	for (std::vector<Primitive*>::iterator tmp = primitives.begin(); tmp != primitives.end(); tmp++)
	{
		RELEASE(*tmp);
	}
	primitives.clear();

	RELEASE(root);

	return true;
}

//PreUpdate
update_status ModuleScene::PreUpdate(float dt)
{
	root->RecursivelyCalculateBbox();

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
	root->Update();

	for (std::vector<Primitive*>::iterator tmp = primitives.begin(); tmp != primitives.end(); tmp++)
	{
		(*tmp)->Render();
	}


	//if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	//{
	//	App->geometry_loader->LoadGeometryFromFile("Assets/warrior.fbx");
	//}

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		App->geometry_loader->LoadGeometryFromFile("Assets/Town/Street environment_V01.fbx", root);
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

void ModuleScene::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

GameObject* ModuleScene::CreateNewGameObject(GameObject* parent, const char* name)
{
	GameObject* ret = new GameObject(parent, name);

	return ret;
}

void ModuleScene::CreateCube(const vec3& position, const vec3& size, float angle, const vec3& rotAxis)
{
	_Cube* c = new _Cube();
	c->size.Set(size.x, size.y, size.z);
	c->SetPos(position.x, position.y, position.z);
	if (angle != 0.0f)
		c->SetRotation(angle, rotAxis);

	else
	{
		primitives.push_back(c);
		App->physics->AddBody(*c, 0);
	}

}

void ModuleScene::CreateSphere(const vec3& position, float radius)
{
	_Sphere* c = new _Sphere();
	c->radius = radius;
	c->SetPos(position.x, position.y, position.z);

	App->physics->AddBody(*c, 0);

	primitives.push_back(c);
}