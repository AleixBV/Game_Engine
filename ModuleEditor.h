#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"

class ModuleEditor : public Module
{
public:

	//Constructor
	ModuleEditor(Application* app, bool start_enabled = true);
	//Destructor
	~ModuleEditor();

	bool Init();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool show_test_window;
	bool show_debug_window;
	bool show_console_window;
	bool show_about_window;

	void ShowDebugWindow(bool* p_open);
	void ShowConsoleWindow(bool* p_open);
};

#endif