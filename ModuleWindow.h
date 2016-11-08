#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool full_desktop);
	void SetBrightness(float brightness);
	void SetWidth(int width);
	uint GetWidth();
	void SetHeight(int height);
	uint GetHeight();
	uint GetRefreshRate();

private:
	bool resizable;
	uint width;
	uint height;
};

#endif // __ModuleWindow_H__