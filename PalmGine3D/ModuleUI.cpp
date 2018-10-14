#include "Application.h"
#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelConfig.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "ModuleUI.h"

ModuleUI::ModuleUI(bool start_enabled)
{
	panels.push_back(about = new PanelAbout());
	panels.push_back(console = new PanelConsole());
	panels.push_back(config = new PanelConfig());
	panels.push_back(inspector = new PanelInspector());
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	LOG("Loading ImGui");

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	config->active = false;

	return true;
}

update_status ModuleUI::Update(float dt)
{
	// IMGUI CODE
	static bool show_test_window = false;

	// Test window
	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}

	// Console
	if (console->active == true) {
		console->Draw();
	}

	//Inspector/Config menu
	if (configActive == true) {
		ImGui::Begin("");
		ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
		if (ImGui::SmallButton("Inspector")) {
			config->active = false;
			inspector->active = true;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Configuration")) {
			config->active = true;
			inspector->active = false;
		}
		ImGui::Separator();
		//Inspector
		if (inspector->active == true) {
			inspector->Draw();
		}
		if (config->active == true) {
			config->Draw();
		}
		ImGui::End();
	}

	// Main Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Show test window"))
			{
				if (show_test_window)
				{
					show_test_window = false;
				}
				else
					show_test_window = true;
			}
			ImGui::Checkbox("Inspector/Config", &configActive);
			ImGui::Checkbox("Console", &console->active);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("About"))
		{
			about->Draw();
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ImGui::Render();
	return UPDATE_CONTINUE;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return(UPDATE_CONTINUE);
}

bool ModuleUI::CleanUp()
{

	for (int i = 0; i < panels.size(); i++) {
		panels.at(i)->~Panel();
	}

	panels.clear();

	LOG("Unloading ImGui");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}