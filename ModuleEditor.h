#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include <vector>

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
	bool show_info_window;
	bool show_config_window;
	bool show_console_window;
	bool show_about_window;

	uint fps_and_ms_log_size;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	void UpdateFpsLog(float new_fps);
	void UpdateMsLog(float new_ms);
	bool vsync;

	void ShowInfoWindow(bool* p_open);
	void ShowConsoleWindow(bool* p_open);
	void ShowConfigWindow(bool* p_open);
};

#endif