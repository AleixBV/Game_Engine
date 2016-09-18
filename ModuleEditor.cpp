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
			if (ImGui::MenuItem("Open test window"))
				show_test_window = !show_test_window;

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