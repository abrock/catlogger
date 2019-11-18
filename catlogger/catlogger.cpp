#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>

#include "catlogger.h"

namespace clog {

namespace  {
static inline std::string rtrim(std::string const &_s) {
    std::string s(_s);
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}
}

Logger &Logger::getInstance()
{
    static Logger    instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
}

Logger::Logger() {}

void Logger::log(const std::string &cat, const int32_t level, const std::string &message) {
    std::string msg = rtrim(message);
    if (!msg.empty()) {
        messages.push_back(std::make_tuple(cat, level, msg));
        for (auto& stream : streams) {
            print(*stream, messages.back());
        }
    }
}

void Logger::log_s(const std::string &cat, const int32_t level, const std::string &message) {
    Logger::getInstance().log(cat, level, message);
}

void Logger::printAll(std::ostream &out, int32_t max_level) const {
    for (auto const& it : messages) {
        if (std::get<1>(it) <= max_level) {
            print(out, it);
        }
    }
}

void Logger::printCat(std::ostream& out, const std::string &cat, int32_t max_level) const {
    for (auto const& it : messages) {
        if (std::get<0>(it) == cat
                && std::get<1>(it) <= max_level) {
            print(out, it);
        }
    }
}

void Logger::clear() {
    messages.clear();
}

void Logger::clear_s() {
    Logger::getInstance().clear();
}

void Logger::print(std::ostream& out, const std::string &cat, const int32_t level, const std::string &message) {
    out << cat << ", " << level << ": " << message << std::endl;
}

void Logger::addListener(std::ostream &out) {
    streams.push_back(&out);
}

template<class T>
void Logger::print(std::ostream & out, const T &tuple) {
    print(out, std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
}

L::Logbuf::Logbuf(const std::string &_cat, const int32_t _level) : cat(_cat), level(_level) {

}

int L::Logbuf::sync() {
    Logger::log_s(cat, level, str());
    str("");
    return 0;
}

} // namespace clog
