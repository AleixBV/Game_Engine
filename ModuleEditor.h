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

	void Draw();

private:
	bool capturing_mouse = false;
	bool capturing_keyboard = false;

	bool show_test_window;
	bool show_hierarchy_window;
	bool show_info_window;
	bool show_config_window;
	bool show_console_window;
	bool show_about_window;

public:
	uint fps_and_ms_log_size;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	void UpdateFpsLog(float new_fps);
	void UpdateMsLog(float new_ms);
	char app_name;
	char organization_name;
	bool vsync;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool full_desktop;
	float brightness;
	int width;
	int height;
	GameObject* game_objetc_selected = nullptr;
	bool new_item_clicked = false;

	void CaptureInput(SDL_Event* input) const;
	bool CapturingMouse() const;
	bool CapturingKeyboard() const;
	void SetWireframeTypeDrawToChilds(GameObject* root, WireframeTypeDraw wireframe_type_draw);

private:
	void ShowHierarchyWindow(bool* show_hierarchy);
	void ShowInfoWindow(bool* show_window);
	void ShowConsoleWindow(bool* show_window);
	void ShowConfigWindow(bool* show_window);

	void AddGameObjectsToHierarchy(GameObject* game_object);
};

#endif