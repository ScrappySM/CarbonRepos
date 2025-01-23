#include "pch.h"

#include "settings.h"

Settings::Settings() {
	ZoneScoped;

	auto oSettingsPath = GetPath();
	if (!oSettingsPath) {
		spdlog::error("Failed to get settings path");
		return;
	}

	auto& settingsPath = oSettingsPath.value();

	if (std::filesystem::exists(settingsPath)) {
		simdjson::dom::parser parser;
		simdjson::padded_string json;

		auto err = simdjson::padded_string::load(settingsPath).get(json);
		if (err) {
			spdlog::error("Failed to load settings file: {}", simdjson::error_message(err));
			return;
		}

		simdjson::dom::element root;

		err = parser.parse(json).get(root);
		if (err) {
			spdlog::error("Failed to parse settings file: {}", simdjson::error_message(err));
			return;
		}

		auto token = root["token"];
		if (!token.error()) {
			strcpy_s(this->token, 256, token.get_string().value().data());
		}

		spdlog::info("Loaded settings file");
	} 
}

Settings::~Settings() {
	this->Save();
}

void Settings::SetToken(const std::string_view& token) {
	strcpy_s(this->token, 256, token.data());
	this->Save();
}

void Settings::Save() const {
	auto data = this->Serialize();
	auto path = this->GetPath();

	if (!path) {
		spdlog::error("Failed to get settings path");
		return;
	}

	if (!std::filesystem::exists(path.value())) {
		spdlog::warn("Settings file does not exist, creating");
	}

	auto& settingsPath = path.value();

	std::ofstream file(settingsPath);

	if (!file.is_open()) {
		spdlog::error("Failed to open settings file");
		return;
	}

	file << data;
	file.close();

	spdlog::info("Saved settings file");
}

std::string Settings::Serialize() const {
	return fmt::format(
R"({{
	"token": "{}"
}})", token);
}

char* Settings::GetToken() {
	return this->token;
}

std::optional<std::string> Settings::GetPath() const {
	try {
		char* appData; size_t len;
		_dupenv_s(&appData, &len, "APPDATA");

		std::filesystem::path dataPath = std::filesystem::path(appData);
		dataPath /= "RepoManager";

		std::filesystem::create_directories(dataPath);
		if (!std::filesystem::exists(dataPath)) {
			spdlog::error("Failed to create data directory");
			return std::nullopt;
		}

		std::filesystem::path settingsPath = dataPath / "settings.json";

		return settingsPath.string();
	}
	catch (const std::exception& e) {
		spdlog::error("Failed to get settings path: {}", e.what());
		return std::nullopt;
	}
}

