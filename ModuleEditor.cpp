#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ComponentTransform.h"
#include "Mesh.h"
#include "ComponentMaterial.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl_gl3.h"
#include "imgui/gl3w.h"

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
	show_hierarchy_window = false;
	show_inspector_window = false;
	show_info_window = false;
	show_config_window = false;
	show_console_window = false;
	show_about_window = false;

	fps_and_ms_log_size = 120;

	//save & load is needed
	vsync = VSYNC;
	fullscreen = WIN_FULLSCREEN;
	brightness = 1.0f;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;

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

	ImGuiIO& io = ImGui::GetIO();
	capturing_keyboard = io.WantCaptureKeyboard;
	capturing_mouse = io.WantCaptureMouse;

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

	if (show_hierarchy_window)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiSetCond_FirstUseEver);
		ShowHierarchyWindow(&show_hierarchy_window);
	}

	if (show_inspector_window)
	{
		ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH * SCREEN_SIZE - 300, 20), ImGuiSetCond_FirstUseEver);
		ShowInspectorWindow(&show_inspector_window);
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
		if (ImGui::Button("Github", ImVec2(100, 25)))
		{
			App->OpenLink("https://github.com/AleixBV/Game_Engine");
		}
		ImGui::Separator();
		ImGui::Text("dear imgui: %s", ImGui::GetVersion());
		SDL_version sdl_version;
		SDL_VERSION(&sdl_version);
		ImGui::Text("SDL version: %u.%u.%u", sdl_version.major, sdl_version.minor, sdl_version.patch);
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
			ImGui::MenuItem("Test Window", "9", &show_test_window);

			ImGui::MenuItem("Hierarchy Window", "1", &show_hierarchy_window);

			ImGui::MenuItem("Inspector Window", "2", &show_inspector_window);

			ImGui::MenuItem("Information Window", "3", &show_info_window);

			ImGui::MenuItem("Configuration Window", "4", &show_config_window);

			ImGui::MenuItem("Console Window", "5", &show_console_window);

			ImGui::MenuItem("About Window", "6", &show_about_window);

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
		show_test_window = !show_test_window;

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		show_hierarchy_window = !show_hierarchy_window;

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		show_inspector_window = !show_inspector_window;

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		show_info_window = !show_info_window;

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		show_config_window = !show_config_window;

	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		show_console_window = !show_console_window;

	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		show_about_window = !show_about_window;

	return ret;
}

void ModuleEditor::Draw()
{
	ImGui::Render();
}

void ModuleEditor::UpdateFpsLog(float new_fps)
{
	uint quantity = fps_log.size();

	if (quantity >= fps_and_ms_log_size)
	{
		for (uint i = 0; i < fps_and_ms_log_size - 1; i++)
		{
			fps_log[i] = fps_log[i + 1];
		}
		quantity--;
		fps_log.pop_back();
	}

	fps_log.push_back(new_fps);
}

void ModuleEditor::UpdateMsLog(float new_ms)
{
	uint quantity = ms_log.size();

	if (quantity >= fps_and_ms_log_size)
	{
		for (uint i = 0; i < fps_and_ms_log_size - 1; i++)
		{
			ms_log[i] = ms_log[i + 1];
		}
		quantity--;
		ms_log.pop_back();
	}

	ms_log.push_back(new_ms);
}

void ModuleEditor::CaptureInput(SDL_Event* input) const
{
	ImGui_ImplSdlGL3_ProcessEvent(input);
}

bool ModuleEditor::CapturingMouse() const
{
	return capturing_mouse;
}
bool ModuleEditor::CapturingKeyboard() const
{
	return capturing_keyboard;
}

void ModuleEditor::SetWireframeTypeDrawToChilds(GameObject* root, WireframeTypeDraw wireframe_type_draw)
{
	for (std::vector<GameObject*>::iterator i = root->children.begin(); i != root->children.end(); i++)
	{
		(*i)->type_draw = wireframe_type_draw;
		for (int x = 0; x < (*i)->children.size(); x++)
		{
			(*i)->children[x]->type_draw = wireframe_type_draw;
			SetWireframeTypeDrawToChilds((*i)->children[x], wireframe_type_draw);
		}
	}
}

void ModuleEditor::ShowHierarchyWindow(bool* show_hierarchy)
{
	if (!ImGui::Begin("Hierarchy", show_hierarchy, ImVec2(300, 750)))
	{
		ImGui::End();
		return;
	}

	if (ImGui::TreeNode(App->scene->root->name.data()))
	{
		AddGameObjectsToHierarchy(App->scene->root);
		ImGui::TreePop();

		if (new_item_clicked && game_object_selected != nullptr)
		{
			App->scene->root->type_draw = WIREFRAME_NORMAL_DRAW;
			SetWireframeTypeDrawToChilds(App->scene->root, WIREFRAME_NORMAL_DRAW);

			game_object_selected->type_draw = WIREFRAME_SELECTED_DRAW;
			SetWireframeTypeDrawToChilds(game_object_selected, WIREFRAME_PARENT_SELECTED_DRAW);

			new_item_clicked = false;
		}
	}

	ImGui::End();
}

void ModuleEditor::ShowInspectorWindow(bool* show_inspector)
{
	if (!ImGui::Begin("Inspector", show_inspector, ImVec2(300, 750)))
	{
		ImGui::End();
		return;
	}

	if (game_object_selected != nullptr)
	{
		for (int i = 0; i < game_object_selected->components.size(); i++)
		{
			switch (game_object_selected->components[i]->type)
			{
			case TRANSFORMATION_COMPONENT:
			{
				ComponentTransform* component_transformation = (ComponentTransform*)game_object_selected->components[i];
				float pos_x = component_transformation->position.x;
				float pos_y = component_transformation->position.y;
				float pos_z = component_transformation->position.z;

				ImGui::Text("Position");
				if (ImGui::InputFloat("x##pos_x", &pos_x, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetPosition(float3(pos_x, pos_y, pos_z));

				if (ImGui::InputFloat("y##pos_y", &pos_y, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetPosition(float3(pos_x, pos_y, pos_z));

				if (ImGui::InputFloat("z##pos_z", &pos_z, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetPosition(float3(pos_x, pos_y, pos_z));

				float rot_x = component_transformation->euler_rot.x;
				float rot_y = component_transformation->euler_rot.y;
				float rot_z = component_transformation->euler_rot.z;

				ImGui::Text("Rotation");
				if (ImGui::InputFloat("x##rot_x", &rot_x, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetRotation(float3(rot_x, rot_y, rot_z));

				if (ImGui::InputFloat("y##rot_y", &rot_y, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetRotation(float3(rot_x, rot_y, rot_z));

				if (ImGui::InputFloat("z##rot_z", &rot_z, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetRotation(float3(rot_x, rot_y, rot_z));

				float scale_x = component_transformation->scale.x;
				float scale_y = component_transformation->scale.y;
				float scale_z = component_transformation->scale.z;

				ImGui::Text("Scale");
				if (ImGui::InputFloat("x##scale_x", &scale_x, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetScale(float3(scale_x, scale_y, scale_z));

				if (ImGui::InputFloat("y##scale_y", &scale_y, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetScale(float3(scale_x, scale_y, scale_z));

				if (ImGui::InputFloat("z##scale_z", &scale_z, 1.0f, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
					game_object_selected->SetScale(float3(scale_x, scale_y, scale_z));
				break;
			}
			case MESH_COMPONENT:
				if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Mesh* mesh = (Mesh*)game_object_selected->components[i];
					ImGui::Text("Indices: %u", mesh->num_indices);
					ImGui::Text("Vertices: %u", mesh->num_vertices);
				}
				break;
			case MATERIAL_COMPONENT:
				if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Text("Texture:");
					ImGui::Image((ImTextureID)((ComponentMaterial*)game_object_selected->components[i])->material_id, ImVec2(100, 100));
				}
				break;
			default:
				break;
			}
		}
	}

	ImGui::End();
}

void ModuleEditor::ShowInfoWindow(bool* show_window)
{
	if (!ImGui::Begin("Information", show_window))
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
		int max_fps = App->GetMaxFps();
		if (ImGui::SliderInt("Max FPS", &max_fps, 0, 120))
			App->SetMaxFps(max_fps);

		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", App->GetMaxFps());
		
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 120.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}

	if (ImGui::CollapsingHeader("Input"))
	{

	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		ImGui::Text("CPUs:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u (Cache: %ukb)", SDL_GetCPUCount(), SDL_GetCPUCacheLineSize());
		ImGui::Text("System RAM:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%.1fMb", (float)SDL_GetSystemRAM());

		ImGui::Text("Caps:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s%s%s%s%s%s%s%s%s%s%s",
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

void ModuleEditor::ShowConfigWindow(bool* show_window)
{
	if (!ImGui::Begin("Configuration", show_window))
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
		static char title[100];
		strcpy_s(title, 100, App->window->GetTitle());
		if (ImGui::InputText("Application Name", title, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->window->SetTitle(title);

		static char organization[100];
		strcpy_s(organization, 100, App->window->GetOrganization());
		if (ImGui::InputText("Organization Name", organization, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->window->SetOrganization(organization);

		if(ImGui::Checkbox("Vsync", &vsync))
		{
			if (vsync)
				if (SDL_GL_SetSwapInterval(1) < 0)
					LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			if (!vsync)
				if (SDL_GL_SetSwapInterval(0) < 0)
					LOG("Warning: Unable to unset VSync! SDL Error: %s\n", SDL_GetError());
		}
	}

	if (ImGui::CollapsingHeader("Window Options"))
	{
		if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
			App->window->SetBrightness(brightness);
		if (ImGui::SliderInt("Width", &width, 640, 1980))
			App->window->SetWidth(width);
		if (ImGui::SliderInt("Height", &height, 480, 1080))
			App->window->SetHeight(height);
		ImGui::Text("Refresh rate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%u", App->window->GetRefreshRate());
		if (ImGui::Checkbox("Fullscreen", &fullscreen))
			App->window->SetFullscreen(fullscreen);
		ImGui::SameLine();
		if (ImGui::Checkbox("Resizable", &resizable))
			App->window->SetResizable(resizable);
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Restart to apply");
		if (ImGui::Checkbox("Borderless", &borderless))
			App->window->SetBorderless(borderless);
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &full_desktop))
			App->window->SetFullDesktop(full_desktop);
	}

	ImGui::End();
}

void ModuleEditor::ShowConsoleWindow(bool* show_window)
{
	//ImGui::Begin("Console", show_window);
	
	//ImGui::End();
}

void ModuleEditor::AddGameObjectsToHierarchy(GameObject* game_object)
{
	for (std::vector<GameObject*>::iterator i = game_object->children.begin(); i != game_object->children.end(); i++)
	{
		uint flags = 0;
		if ((*i)->children.size() == 0)
			flags = ImGuiTreeNodeFlags_Leaf;
		if (ImGui::TreeNodeEx((*i)->name.data(), flags))
		{
			CheckClickInHierarchy(*i);
			AddGameObjectsToHierarchy(*i);
			ImGui::TreePop();
		}
		else
			CheckClickInHierarchy(*i);
	}
}

void ModuleEditor::CheckClickInHierarchy(GameObject* game_object)
{
	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseClicked(0))
		{
			if (game_object_selected != game_object)
			{
				new_item_clicked = true;
				game_object_selected = game_object;
			}
		}

		if (ImGui::IsMouseDoubleClicked(0))
		{
			float3 position;
			if (game_object->GetGlobalPosition(&position))
			{
				App->camera->LookAt(vec3(position.x, position.y, position.z));
				App->camera->Position = vec3(position.x, position.y, position.z) + App->camera->Z * 50.0f;
				App->camera->Reference = vec3(position.x, position.y, position.z) + App->camera->Z * 50.0f;
			}
		}
	}
}
