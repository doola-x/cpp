#pragma once
#include <string>
#include <unordered_map>

class Logger {
	std::unordered_map<std::string, int> book;
public:
    Logger() {
    }

    bool shouldPrintMessage(int timestamp, const std::string& message) {
	auto val = book.find(message);
	if (val == book.end()) {
		book[message] = timestamp;
		return true;
	}
	if (timestamp - val->second  >= 10) {
		book[message] = timestamp;
		return true;
	}
        return false;
    }
};
