#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
		//save % load is needed
		resizable = WIN_RESIZABLE;
		width = SCREEN_WIDTH * SCREEN_SIZE;
		height = SCREEN_HEIGHT * SCREEN_SIZE;
		title = TITLE;
		organization = "CITM";

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(resizable == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* _title)
{
	SDL_SetWindowTitle(window, _title);
	title = _title;
}

const char* ModuleWindow::GetTitle() const
{
	return (char*)title;
}

void ModuleWindow::SetOrganization(const char* _organization)
{
	SDL_SetWindowTitle(window, _organization);
	organization = _organization;
}

const char* ModuleWindow::GetOrganization() const
{
	return (char*)organization;
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	if (fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetResizable(bool _resizable)
{
	resizable = _resizable; //save & load is needed!
}

void ModuleWindow::SetBorderless(bool borderless)
{
	SDL_SetWindowBordered(window, (SDL_bool)!borderless);
}

void ModuleWindow::SetFullDesktop(bool full_desktop)
{
	if (full_desktop)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(window, 0);
}

void ModuleWindow::SetBrightness(float brightness)
{
	SDL_SetWindowBrightness(window, brightness);
}

void ModuleWindow::SetWidth(int _width)
{
	width = _width * SCREEN_SIZE;
	SDL_SetWindowSize(window, width, height);
	App->renderer3D->OnResize(width, height);
}

uint ModuleWindow::GetWidth()
{
	return width;
}

void ModuleWindow::SetHeight(int _height)
{
	height = _height * SCREEN_SIZE;
	SDL_SetWindowSize(window, width, height);
	App->renderer3D->OnResize(width, height);
}

uint ModuleWindow::GetHeight()
{
	return height;
}

uint ModuleWindow::GetRefreshRate()
{
	SDL_DisplayMode display_mode;
	SDL_GetDesktopDisplayMode(0, &display_mode);

	return display_mode.refresh_rate;
}