#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
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
		static char* title = (char*)App->window->GetTitle();
		if (ImGui::InputText("Application Name", title, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->window->SetTitle(title);

		static char* organization = (char*)App->window->GetOrganization();
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
		if (ImGui::SliderInt("Width", &width, 1, 1980))
			App->window->SetWidth(width);
		if (ImGui::SliderInt("Height", &height, 1, 1080))
			App->window->SetHeight(height);
		ImGui::Text("Refresh rate: %u", App->window->GetRefreshRate());
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

void ModuleEditor::ShowConsoleWindow(bool* p_open)
{

}
