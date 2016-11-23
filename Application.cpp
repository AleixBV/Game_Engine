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
		ret = true;
		app_config = config.child("app");
		title = app_config.child("title").child_value(DEFAULT_TITLE);
		organization = app_config.child("organization").child_value("CITM");

		max_fps = app_config.attribute("max_fps").as_int(120);
		max_ms_per_frame = 1000.0f / max_fps;

		std::list<Module*>::iterator item = list_modules.begin();

		while (item != list_modules.end() && ret == true)
		{
			ret = (*item)->Awake(config.child((*item)->GetName().c_str()));
			item++;
		}
	}
	else
	{
		// self-config
		ret = true;
		title = DEFAULT_TITLE;
		organization = "CITM";

		max_fps = 120;
		max_ms_per_frame = 1000.0f / max_fps;
	}

	return ret;
}

bool Application::Init()
{
	bool ret = true;
	want_to_save_config = false;
	want_to_save_game = false;
	game_save_file = nullptr;
	frames = 0.0f;
	ms_in_last_frame = 0.0f;

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
	if (size > 0)
	{
		pugi::xml_parse_result result = config_file.load_buffer(buf, size);
		RELEASE(buf);

		if (result == NULL)
		{
			LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		}
		else
			ret = config_file.child("config");
	}

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
	if (want_to_save_config)
	{
		SaveConfigNow();
		want_to_save_config = false;
	}

	if (want_to_save_game)
	{
		SaveGameNow();
		want_to_save_game = false;
		game_save_file = nullptr;
	}

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

void Application::SaveConfig()
{
	want_to_save_config = true;
}

void Application::SaveGame(const char* file)
{
	want_to_save_game = true;
	game_save_file = file;
}

bool Application::SaveConfigNow() const
{
	bool ret = true;

	LOG("Saving Config");

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("config");

	//save Application
	pugi::xml_node cam = data.append_child("application");

	cam.append_attribute("title") = title;
	cam.append_attribute("organization") = organization;
	cam.append_attribute("max_fps") = max_fps;

	//save Modules
	std::list<Module*>::const_iterator item = list_modules.begin();
	for(item; item != list_modules.end(); item++)
	{
		ret = (*item)->SaveConfig(root.append_child((*item)->GetName().c_str()));
	}

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		// we are done, so write data to disk
		file_system->Save("config.xml", stream.str().c_str(), stream.str().length());
		LOG("... finished saving config.xml");
	}
	else
		LOG("Save config process halted from an error");

	data.reset();
	return ret;
}

bool Application::SaveGameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", game_save_file);

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	std::list<Module*>::const_iterator item = list_modules.begin();
	for (item; item != list_modules.end(); item++)
	{
		ret = (*item)->SaveConfig(root.append_child((*item)->GetName().c_str()));
	}

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		// we are done, so write data to disk
		file_system->Save(game_save_file, stream.str().c_str(), stream.str().length());
		LOG("... finished saving ", game_save_file);
	}
	else
		LOG("Save game process halted from an error");

	data.reset();
	return ret;
}