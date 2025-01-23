#include "github.h"

GitHub::GitHub() {
	curl_global_init(CURL_GLOBAL_ALL);
	this->curl = curl_easy_init();
	curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, WriteCallback);
}

GitHub::~GitHub() {
	curl_easy_cleanup(this->curl);
	curl_global_cleanup();
}

void GitHub::SetToken(const std::string_view& token) {
	memset(this->token, 0, sizeof(this->token));
	memcpy(this->token, token.data(), token.size());
}

Response GitHub::MakeRequest(const std::string_view& url, bool useToken) {
	Response response = { 0, "" };

	if (this->curl) {
		struct curl_slist *headers = NULL;
		if (useToken) {
		    std::string authHeader = fmt::format("Authorization: Bearer {}", this->token);
		    headers = curl_slist_append(headers, authHeader.c_str());

			std::string userAgent = fmt::format("User-Agent: {}", "RepoManager");
			headers = curl_slist_append(headers, userAgent.c_str());

			curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
		}

		curl_easy_setopt(this->curl, CURLOPT_URL, url.data());
		curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &response.body);

		CURLcode res = curl_easy_perform(this->curl);
		if (res != CURLE_OK) {
			spdlog::error("curl_easy_perform() failed: {}", curl_easy_strerror(res));
		}

		curl_easy_getinfo(this->curl, CURLINFO_RESPONSE_CODE, &response.code);
	}

	return response;
}

size_t GitHub::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
	size_t realSize = size * nmemb;
	userp->append(static_cast<char*>(contents), realSize);
	return realSize;
}
