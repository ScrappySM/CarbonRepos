#pragma once

#include "pch.h"

class NotifySink : public spdlog::sinks::sink {
public:
	void log(const spdlog::details::log_msg& msg) override {
		ZoneScoped;

		switch (msg.level) {
		case spdlog::level::trace:
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType::Info, msg.payload.data()));
			break;
		case spdlog::level::debug:
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType::Info, msg.payload.data()));
			break;
		case spdlog::level::info:
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType::Info, msg.payload.data()));
			break;
		case spdlog::level::warn:
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType::Warning, msg.payload.data()));
			break;
		case spdlog::level::err:
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType::Error, msg.payload.data()));
			break;
		case spdlog::level::critical:
			ImGui::InsertNotification(ImGuiToast(ImGuiToastType::Error, msg.payload.data()));
			break;
		case spdlog::level::off:
			break;
		default:
			break;
		}

		const char* msgStr = msg.payload.data();
		tracy::Profiler::Message(msgStr, msg.payload.size(), 0);

		// Call a standard spdlog colour sink
		static spdlog::sinks::stdout_color_sink_mt sink;
		sink.log(msg);
	}

	void flush() override {}
	void set_pattern(const std::string &pattern) override {}
	void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override {}
};
