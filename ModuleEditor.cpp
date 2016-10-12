#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_sdl_gl3.h"
#include "imgui\gl3w.h"

//Constructor
ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

//Destructor
ModuleEditor::~ModuleEditor()
{

}

bool ModuleEditor::Init()
{
	LOG("Init editor gui with imgui lib version %s", ImGui::GetVersion());

	gl3wInit();
	
	ImGui_ImplSdlGL3_Init(App->window->window);

	show_test_window = false;
	show_info_window = false;
	show_config_window = false;
	show_console_window = false;
	show_about_window = false;

	return true;
}

bool ModuleEditor::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();

	return true;
}

update_status ModuleEditor::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleEditor::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(1000, 100), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}

	if (show_info_window)
	{
		ImGui::SetNextWindowPos(ImVec2(500, 50), ImGuiSetCond_FirstUseEver);
		ShowInfoWindow(&show_info_window);
	}

	if (show_config_window)
	{
		ImGui::SetNextWindowPos(ImVec2(500, 50), ImGuiSetCond_FirstUseEver);
		ShowConfigWindow(&show_config_window);
	}

	if (show_console_window)
	{
		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_FirstUseEver);
		ShowConsoleWindow(&show_console_window);
	}

	if (show_about_window)
	{
		ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("About", &show_about_window, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Game Engine made by Aleix Borrell Vives.");
		ImGui::Separator();
		ImGui::Text("The Game Engine is This is free and unencumbered software released into the public domain for educational purposes. see License in the README for more information.");
		ImGui::Separator();
		ImGui::Text("dear imgui, %s", ImGui::GetVersion());
		ImGui::End();
	}

	if (ImGui::BeginMainMenuBar())
	{
		bool selected = false;

		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New");
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Load");

			if (ImGui::MenuItem("Quit"))
				ret = UPDATE_STOP;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Test Window", NULL, &show_test_window);

			ImGui::MenuItem("Information Window", NULL, &show_info_window);

			ImGui::MenuItem("Configuration Window", NULL, &show_config_window);

			ImGui::MenuItem("Console Window", NULL, &show_console_window);

			ImGui::MenuItem("About Window", NULL, &show_about_window);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


	//--------------------
	ImGui::Render();
	return ret;
}

update_status ModuleEditor::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;

}

void ModuleEditor::ShowInfoWindow(bool* p_open)
{
	if (!ImGui::Begin("Information", p_open))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenu("Close"))
	{
		show_info_window = false;
		ImGui::EndMenu();
	}

	ImGui::PushItemWidth(-140);

	ImGui::Text("Information Window.");

	if (ImGui::CollapsingHeader("Application info"))
	{

	}

	if (ImGui::CollapsingHeader("Input"))
	{

	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPUs: %u (Cache: %ukb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM: %.1fMb", SDL_GetSystemRAM());

		ImGui::Text("Caps:%s%s%s%s%s%s%s%s%s%s%s",
			SDL_Has3DNow() ? " 3DNow," : "",
			SDL_HasAVX() ? " AVX," : "",
			SDL_HasAVX2() ? " AVX2" : "",
			SDL_HasAltiVec() ? " AltiVec," : "",
			SDL_HasMMX() ? " MMX," : "",
			SDL_HasRDTSC() ? " RDTSC," : "",
			SDL_HasSSE() ? " SSE," : "",
			SDL_HasSSE2() ? " SSE2," : "",
			SDL_HasSSE3() ? " SSE3," : "",
			SDL_HasSSE41() ? " SSE41," : "",
			SDL_HasSSE42() ? " SSE42," : "");
	}

	ImGui::End();
}

void ModuleEditor::ShowConfigWindow(bool* p_open)
{
	if (!ImGui::Begin("Configuration", p_open))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenu("Close"))
	{
		show_config_window = false;
		ImGui::EndMenu();
	}

	ImGui::PushItemWidth(-140);

	ImGui::Text("Configuration Window.");

	if (ImGui::CollapsingHeader("Application"))
	{

	}

	if (ImGui::CollapsingHeader("Window Options"))
	{

	}

	ImGui::End();
}

void ModuleEditor::ShowConsoleWindow(bool* p_open)
{

}
