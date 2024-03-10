#include <iostream>
#include <string>
#include <curl/curl.h>

// istediginiz vanity buraya
const std::string DESIRED_VANITY_URL = "sunucuadi";

// istediginiz vanitydeki listener token buraya
const std::string LISTENER_TOKEN = "listenertoken";

// vanity bos ise alicak hesabin token buraya
const std::string STEALER_TOKEN = "stealertoken";

// vanityi alacaginiz boostlu server idsi
const std::string SERVER_ID = "1231232131312";

// vanity bosmu diye bakilacak sure default 60
const int INTERVAL = 60;

// sunucudan gelen yanıtı işlemek için callbak işlevi
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// sunucunun vanity URL'sine bakmak icin kod
std::string get_vanity_url(const std::string& token) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        std::string url = "https://discord.com/api/v9/guilds/" + SERVER_ID + "/vanity-url";
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bot " + token).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
		//ec
    }

    size_t pos = readBuffer.find("\"code\":\"") + 8;
    std::string vanityUrl = readBuffer.substr(pos, readBuffer.find("\"", pos) - pos);
    return vanityUrl;
}

// almak istenen vanity URL'sini deneme islevi
bool claim_vanity_url(const std::string& token) {
    CURL* curl = curl_easy_init();

    if (curl) {
        std::string url = "https://discord.com/api/v9/guilds/" + SERVER_ID + "/vanity-url";
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bot " + token).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        struct curl_httppost* formpost = NULL;
        struct curl_httppost* lastptr = NULL;
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "code", CURLFORM_COPYCONTENTS, DESIRED_VANITY_URL.c_str(), CURLFORM_END);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            long http_code = 0;
            curl
			// dont stole all just fix and continue coding :D