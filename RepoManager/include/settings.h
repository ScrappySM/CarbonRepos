#pragma once

#include "pch.h"

class Settings {
public:
	Settings();
	~Settings();

	Settings(const Settings&) = delete;
	Settings& operator=(const Settings&) = delete;

	void SetToken(const std::string_view& token);
	char* GetToken();

	std::string Serialize() const;
	void Save() const;

	std::optional<std::string> GetPath() const;

private:
	char token[256] = { 0 };
};
