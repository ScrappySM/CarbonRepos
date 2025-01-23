#pragma once

#include "pch.h"

#include "idler.h"
#include "github.h"
#include "settings.h"
#include "repo.h"

enum class Tab {
	Discover,
	Console,
	Settings,
};

class State {
public:
	// GUI State
	Tab currentTab = Tab::Discover;
	Idler idler;

	Settings settings;
	GitHub gitHub;
	Repo repo = Repo(mockRepoJSON);

public:
	static State* GetInstance() {
		ZoneScoped;
		static State instance;
		return &instance;
	}

private:
	State() = default;
	~State() = default;

	State(const State&) = delete;
	State& operator=(const State&) = delete;
};
