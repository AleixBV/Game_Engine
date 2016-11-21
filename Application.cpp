#include "Application.h"

Application::Application()
{
	file_system = new ModuleFileSystem(this);
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene = new ModuleScene(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	geometry_loader = new ModuleGeometryLoader(this);
	editor = new ModuleEditor(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(file_system);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);
	AddModule(geometry_loader);
	
	// Scenes
	AddModule(scene);

	// Renderer last!
	AddModule(renderer3D);
	AddModule(editor);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete *item;
		item++;
	}
}

bool Application::Awake()
{
	bool ret = false;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	config = LoadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title = app_config.child("title").child_value();
		organization = app_config.child("organization").child_value();

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		//if (cap > 0)
		//{
		//	capped_ms = 1000 / cap;
		//}
	}

	if (ret == true)
	{
		std::list<Module*>::iterator item = list_modules.begin();

		while (item != list_modules.end() && ret == true)
		{
			ret = (*item)->Awake(config.child((*item)->GetName().c_str()));
			item++;
		}
	}

	return ret;
}

bool Application::Init()
{
	bool ret = true;
	max_fps = 120;
	max_ms_per_frame = 1000.0f / 120.0f;
	frames = 0.0f;
	ms_in_last_frame = 0.0f;
	title = TITLE;
	organization = "CITM";

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	
	ms_timer.Start();
	fps_timer.Start();
	return ret;
}

pugi::xml_node Application::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = file_system->Load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	}
	else
		ret = config_file.child("config");

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	frames++;

	if (fps_timer.Read() >= 1000)
	{
		editor->UpdateFpsLog(frames);
		frames = 0;
		fps_timer.Start();
	}

	ms_in_last_frame = ms_timer.Read();

	if (ms_in_last_frame < max_ms_per_frame && max_ms_per_frame > 0)
		SDL_Delay(max_ms_per_frame - ms_in_last_frame);

	editor->UpdateMsLog(ms_in_last_frame);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

void Application::SetTitle(const char* _title)
{
	SDL_SetWindowTitle(window->window, _title);
	title = _title;
}

const char* Application::GetTitle() const
{
	return (char*)title;
}

void Application::SetOrganization(const char* _organization)
{
	SDL_SetWindowTitle(window->window, _organization);
	organization = _organization;
}

const char* Application::GetOrganization() const
{
	return (char*)organization;
}

int Application::GetMaxFps()
{
	return max_fps;
}

void Application::SetMaxFps(int x)
{
	if (x > 0 && x < 120)
		max_fps = x;
	else
		max_fps = 120;

	max_ms_per_frame = 1000 / (float)max_fps;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::OpenLink(char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 3);
}

void Application::DebugDraw()
{
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		(*item)->DebugDraw();
		item++;
	}
}