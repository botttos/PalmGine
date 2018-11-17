#include "Application.h"
#include "PanelTimeManagement.h"

PanelTimeManagement::PanelTimeManagement() : Panel("TimeManagement")
{
	active = true;
}
PanelTimeManagement::~PanelTimeManagement()
{
}

//Function that draws all the console
void PanelTimeManagement::Draw()
{
	//Creates the console interface
	ImGui::Begin("Time Management", &active, 
		ImGuiWindowFlags_HorizontalScrollbar | 
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse);

	ImGui::SetWindowPos({(250.0f*App->window->width)/1280, 20.0f});
	ImGui::SetWindowSize(ImVec2((500*App->window->width)/1280, (60*App->window->height)/1024), ImGuiCond_Always);

	//Buttons
	if (ImGui::Button("Play")) {
		if (App->scene_intro->game_running == false) {
			App->scene_intro->in_game_timer.Start();
			App->scene_intro->game_running = true;
			App->scene_intro->SerializeScene("AutoSaveWhenPlay");
		}
	}
	ImGui::SameLine();
	if (App->scene_intro->in_game_timer.IsPaused() == false) {
		if (ImGui::Button("Pause")) {
			if (App->scene_intro->game_running == true) {
				App->scene_intro->in_game_timer.Pause();
			}
		}
	}
	else {
		if (ImGui::Button("Continue")) {
			if (App->scene_intro->game_running == true) {
				App->scene_intro->in_game_timer.UnPause();
			}
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop")) {
		if (App->scene_intro->game_running == true) {
			App->scene_intro->in_game_timer.Start();
			App->scene_intro->in_game_timer.Stop();
			App->scene_intro->game_running = false;
			App->scene_intro->LoadSceneData("AutoSaveWhenPlay");
			std::string bin_path;
			bin_path = BINARY_SCENE_PATH;
			bin_path += "AutoSaveWhenPlay";
			bin_path += ".binaryscene";
			remove(bin_path.c_str());
			bin_path.clear();
			App->scene_intro->in_game_timer.ResetExtraTicks();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("NextFrame")) {
		if (App->scene_intro->game_running == true){
			App->scene_intro->in_game_timer.AddExtraTicks(App->scene_intro->in_game_timer.GetDT());
		}
	}
	ImGui::SameLine();

	//TimeScale Slider
	ImGui::PushItemWidth(150);
	if (ImGui::SliderFloat("Time Scale", &App->scene_intro->in_game_time_scale, 0.0f, 2.0f, "%.1f")) {
		App->scene_intro->in_game_timer.SetTimerScale(App->scene_intro->in_game_time_scale);
	}

	ImGui::End();
}