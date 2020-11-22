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

Logger::Logger() {
     start_time = std::chrono::high_resolution_clock::now();
}

void Logger::clearTmpListeners() {
    tmp_listeners.clear();
}

void Logger::log(const std::string &cat, const int32_t level, const std::string &message) {
    m.lock();
    std::string msg = rtrim(message);
    if (!msg.empty()) {
        if (log_timestamps) {
            messages.push_back(std::make_tuple(getTime() + " " + cat, level, msg));
        }
        else {
            messages.push_back(std::make_tuple(cat, level, msg));
        }
        for (auto& stream : listeners) {
            print(*stream, messages.back());
        }
        for (auto& stream : tmp_listeners) {
            print(*stream, messages.back());
        }
    }
    m.unlock();
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
    listeners.push_back(&out);
}

void Logger::addTmpListener(std::ostream &out) {
    tmp_listeners.push_back(&out);
}

void Logger::logTimestamps(bool log) {
    log_timestamps = log;
}

std::string Logger::getTime() {
    using namespace std::chrono;

    // get current time
    auto now = system_clock::now();

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = system_clock::to_time_t(now);

    // convert to broken time
    std::tm bt = *std::localtime(&timer);

    std::ostringstream oss;

    oss << std::put_time(&bt, "%H-%M-%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}

double Logger::timeSinceStart() {
    return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - getInstance().start_time).count();
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
