#pragma once

#include "pch.h"

enum class ModTarget {
	GAME,
	MODTOOL
};

struct Mod {
	struct {
        std::string user = "";
		std::string repo = "";
	} gitHub = {};
    
    std::string name;
	std::string description;
	std::string version;

	std::vector<std::string> verifiedCommits = {};

    std::map<std::string, std::vector<std::string>> downloadUrls = {};

	std::vector<std::string> authors = {};
};

class Repo {
public:
	Repo(const std::string_view& json);
	~Repo();

	Repo(const Repo&) = delete;
	Repo& operator=(const Repo&) = delete;

	std::vector<Mod>& GetMods(ModTarget target);

private:
	void LoadRepo(const std::string_view& json);

	std::vector<Mod> gameMods;
    std::vector<Mod> modtoolMods;

    std::mutex mtx;
};

static constexpr const char* mockRepoJSON = R"(
{
  "game": [
    {
      "name": "SM-BetterPaintTool",
      "description": "A better paint tool for Scrap Mechanic",
      "version": "1.0.0",
      "authors": [
        "QuestionableM"
      ],
      "github": {
        "user": "QuestionableM",
        "repo": "SM-BetterPaintTool"
      },
      "verifiedCommits": [
        "adbasn",
        "asdasd"
      ],
      "downloadUrls": {
        "latest": [
          "https://github.com/1",
          "https://github.com/2",
          "https://github.com/3",
          "https://github.com/4"
        ],
        "adbasn": [
          "https://github.com/1",
          "https://github.com/2",
          "https://github.com/3",
          "https://github.com/4"
        ]
      }
    }
  ],
  "modtool": [
    {
      "name": "SM-NoManyFileDialogs",
      "description": "Removes the file dialog bug where it opens too many dialogs",
      "version": "1.0.0",
      "authors": [
        "BenMcAvoy"
      ],
      "github": {
        "user": "BenMcAvoy",
        "repo": "SM-NoManyFileDialogs"
      },
      "verifiedCommits": [
        "adbasn",
        "asdasd"
      ],
      "downloadUrls": {
        "latest": [
          "https://github.com/1",
          "https://github.com/2",
          "https://github.com/3",
          "https://github.com/4"
        ],
        "adbasn": [
          "https://github.com/1",
          "https://github.com/2",
          "https://github.com/3",
          "https://github.com/4"
        ]
      }
    }
  ]
}
)";