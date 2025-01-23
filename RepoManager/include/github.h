#pragma once

#include "pch.h"

struct Response {
	int code;
	std::string body;
};

class GitHub {
public:
	GitHub();
	~GitHub();

	void SetToken(const std::string_view& token);

	Response MakeRequest(const std::string_view& url, bool useToken = true);

private:
	char token[256] = { 0 };

	// Curl callback
	static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

	CURL* curl = nullptr;
};
