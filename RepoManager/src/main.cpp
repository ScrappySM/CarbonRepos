#include "pch.h"

#include "window.h"
#include "state.h"
#include "log.h"
#include "github.h"

#include "font/IconsFontAwesome6.h"

static void Render(Window& window) {
	ZoneScoped;

	static State* state = State::GetInstance();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();

	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2((float)window.width, (float)window.height));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Repo Manager", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar);

	bool shouldOpenAboutPopup = false;
	bool shouldOpenSettingsPopup = false;
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Input GitHub PAT")) {
				shouldOpenSettingsPopup = true;
			}

			if (ImGui::MenuItem("Exit")) {
				glfwSetWindowShouldClose(window.GetContext(), GLFW_TRUE);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("About")) {
				shouldOpenAboutPopup = true;
			}
			ImGui::EndMenu();
		}
	}

	ImGui::EndMenuBar();

	if (shouldOpenAboutPopup) {
		ImGui::OpenPopup("About");
	}

	if (shouldOpenSettingsPopup) {
		ImGui::OpenPopup("Settings");
	}

	if (ImGui::BeginPopupModal("About", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Repo Manager, for managing SM DLL modding repos");
		ImGui::Text("Version 1.0.0");
		ImGui::Text("By");
		ImGui::SameLine();
		if (ImGui::Button("BenMcAvoy")) {
			ShellExecuteA(nullptr, "open", "https://github.com/BenMcAvoy", nullptr, nullptr, SW_SHOWNORMAL);
		}
		ImGui::SameLine();
		ImGui::Text("for");
		ImGui::SameLine();
		if (ImGui::Button("ScrappySM")) {
			ShellExecuteA(nullptr, "open", "https://github.com/ScrappySM", nullptr, nullptr, SW_SHOWNORMAL);
		}
		ImGui::Separator();
		if (ImGui::Button("Close")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Input your GitHub token here:");
		ImGui::InputText("Token", state->settings.GetToken(), 256);
		ImGui::SameLine();

		if (ImGui::Button("Done")) {
			state->settings.Save();
			state->gitHub.SetToken(state->settings.GetToken());

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	static float windowPadding = ImGui::GetStyle().WindowPadding.x;
	float availableArea = ImGui::GetWindowContentRegionMax().x;
	float ModsWidth = availableArea * (1.0f / 3.0f) - windowPadding;
	float DataWidth = availableArea * (2.0f / 3.0f) - windowPadding;

	ImGui::BeginChild("Mods", ImVec2(ModsWidth, 0), true);
	
	for (auto& mod : state->repo.GetMods(ModTarget::GAME)) {
		ImGui::BeginChild(mod.name.c_str(), ImVec2(0, 100), true);
		ImVec2 cursorPos = ImGui::GetCursorPos();
		float childWidth = ImGui::GetContentRegionAvail().x;
		float textSize = ImGui::CalcTextSize("View").x;

		ImGui::SetWindowFontScale(1.1);	
		ImGui::Text(ICON_FA_PUZZLE_PIECE " %s", mod.name.c_str());
		ImGui::SetWindowFontScale(1);
		ImGui::TextWrapped(mod.description.c_str());

		ImGui::SetCursorPos(ImVec2((cursorPos.x + childWidth) - textSize, cursorPos.y));

		if (ImGui::Button("View", ImVec2(textSize, 0))) {}

		ImGui::EndChild();
	}

	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Data", ImVec2(DataWidth, 0), true);

	if (ImGui::Button("Test GitHub API")) {
		state->gitHub.SetToken(state->settings.GetToken());
		auto response = state->gitHub.MakeRequest("https://api.github.com/versions", true);
		spdlog::info("Response code: {}", response.code);
		spdlog::info("Response body: {}", response.body);
	}

	if (ImGui::Button("Hello")) {
		spdlog::info("Hello");
	}

	ImGui::EndChild();

	ImGui::End();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f)); // Background color
	ImGui::RenderNotifications();
	ImGui::PopStyleColor();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main(void) {
	ZoneScoped;

	Window window("Repo Manager", 1280, 720);

	auto logger = std::make_shared<NotifySink>();
	spdlog::set_default_logger(std::make_shared<spdlog::logger>("default", logger));

	static State* state = State::GetInstance();
	static auto clearColour = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

	while (!glfwWindowShouldClose(window.GetContext())) {
		ZoneScopedN("Main Loop");

		state->idler.IdleBySleeping();

		glfwPollEvents();

		glClearColor(clearColour.x, clearColour.y, clearColour.z, clearColour.w);
		glClear(GL_COLOR_BUFFER_BIT);

		Render(window);

		glfwSwapBuffers(window.GetContext());

		static ImGuiIO& io = ImGui::GetIO();
		TracyPlot("FPS", io.Framerate);

		TracyGpuCollect;
		FrameMark;
	}

	return 0;
}