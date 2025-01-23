#pragma once

#pragma comment(lib, "dwmapi.lib")

#define WIN32_LEAN_AND_MEAN
#define GLFW_INCLUDE_NONE

#define TRACY_ENABLE

#include <Windows.h>
#include <TlHelp32.h>
#include <dwmapi.h>
#include <shellapi.h>

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <thread>
#include <chrono>
#include <fstream>
#include <mutex>
#include <unordered_map>

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuiNotify.hpp>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <spdlog/sinks/sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <fmt/format.h>

#include <simdjson.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>
#include <tracy/TracyC.h>

#include <curl/curl.h>
#include <curl/easy.h>
