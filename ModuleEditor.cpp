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
	show_debug_window = false;
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

	if (show_debug_window)
	{
		ImGui::SetNextWindowPos(ImVec2(500, 50), ImGuiSetCond_FirstUseEver);
		ShowDebugWindow(&show_debug_window);
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

			ImGui::MenuItem("Debug Window", NULL, &show_debug_window);

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

void ModuleEditor::ShowDebugWindow(bool* p_open)
{
	if (!ImGui::Begin("Debug", p_open))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenu("Close"))
	{
		show_debug_window = false;
		ImGui::EndMenu();
	}

	ImGui::PushItemWidth(-140);

	ImGui::Text("Debug Window.");

	if (ImGui::CollapsingHeader("Application info"))
	{

	}

	if(ImGui::CollapsingHeader("Window Options"))
	{

	}

	if (ImGui::CollapsingHeader("Input"))
	{

	}

	if (ImGui::CollapsingHeader("Hardware"))
	{

	}

	ImGui::End();
}

void ModuleEditor::ShowConsoleWindow(bool* p_open)
{

}
