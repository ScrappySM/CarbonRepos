#include "pch.h"

#include "repo.h"
#include "state.h"

void Repo::LoadRepo(const std::string_view& jsonIn) {
	ZoneScoped;

	tracy::SetThreadName("Repo::LoadRepo");

	simdjson::dom::parser parser;
	simdjson::dom::element root;
	std::string_view json;

	auto err = parser.parse(jsonIn).get(root);
	if (err) {
		spdlog::error("Failed to pass repo json: {}", simdjson::error_message(err));
		return;
	}

	spdlog::info("Loaded repo json");

	auto gameMods = root["game"];

	for (simdjson::dom::element mod : gameMods) {
		Mod newMod;
		auto name = mod["name"];
		if (!name.error()) {
			newMod.name = name.get_string().value().data();
		}
		auto description = mod["description"];
		if (!description.error()) {
			newMod.description = description.get_string().value().data();
		}
		auto version = mod["version"];
		if (!version.error()) {
			newMod.version = version.get_string().value().data();
		}
		auto authors = mod["authors"];
		if (!authors.error()) {
			for (simdjson::dom::element author : authors) {
				newMod.authors.push_back(author.get_string().value().data());
			}
		}
		auto gitHub = mod["github"];
		if (!gitHub.error()) {
			auto user = gitHub["user"];
			if (!user.error()) {
				newMod.gitHub.user = user.get_string().value().data();
			}
			auto repo = gitHub["repo"];
			if (!repo.error()) {
				newMod.gitHub.repo = repo.get_string().value().data();
			}
		}
		auto verifiedCommits = mod["verifiedCommits"];
		if (!verifiedCommits.error()) {
			for (simdjson::dom::element commit : verifiedCommits) {
				newMod.verifiedCommits.push_back(commit.get_string().value().data());
			}
		}

		this->gameMods.push_back(newMod);
	}

	auto modtoolMods = root["modtool"];

	for (simdjson::dom::element mod : modtoolMods) {
		Mod newMod;
		auto name = mod["name"];
		if (!name.error()) {
			newMod.name = name.get_string().value().data();
		}
		auto description = mod["description"];
		if (!description.error()) {
			newMod.description = description.get_string().value().data();
		}
		auto version = mod["version"];
		if (!version.error()) {
			newMod.version = version.get_string().value().data();
		}
		auto authors = mod["authors"];
		if (!authors.error()) {
			for (simdjson::dom::element author : authors) {
				newMod.authors.push_back(author.get_string().value().data());
			}
		}
		auto gitHub = mod["github"];
		if (!gitHub.error()) {
			auto user = gitHub["user"];
			if (!user.error()) {
				newMod.gitHub.user = user.get_string().value().data();
			}
			auto repo = gitHub["repo"];
			if (!repo.error()) {
				newMod.gitHub.repo = repo.get_string().value().data();
			}
		}
		auto verifiedCommits = mod["verifiedCommits"];
		if (!verifiedCommits.error()) {
			for (simdjson::dom::element commit : verifiedCommits) {
				newMod.verifiedCommits.push_back(commit.get_string().value().data());
			}
		}

		this->modtoolMods.push_back(newMod);
	}
}

Repo::Repo(const std::string_view& jsonIn) {
	std::thread t(&Repo::LoadRepo, this, jsonIn);
	t.detach();
}

Repo::~Repo() {}

std::vector<Mod>& Repo::GetMods(ModTarget modTarget) {
	if (modTarget == ModTarget::GAME) {
		return this->gameMods;
	}
	else {
		return this->modtoolMods;
	}
}