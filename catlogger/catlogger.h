#ifndef CATLOGGER_H
#define CATLOGGER_H

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>
#include <vector>
#include <map>
#include <mutex>
#include <chrono>
#include <iomanip>

namespace clog {

class L : public std::ostream {
private:
    class Logbuf : public std::stringbuf {
    private:
        std::string const& cat;
        int32_t const level;
    public:
        Logbuf(std::string const& _cat, int32_t const _level);
        ~Logbuf() {  pubsync(); }
        int sync();
    };

public:
    L(std::string const& _cat, int32_t const _level) : std::ostream(new Logbuf(_cat, _level)) {}
    ~L() { delete rdbuf(); }
};

class Logger {
public:
    static Logger& getInstance();
private:
    Logger();

    /**
     * @brief messages All recorded messages.
     * Each tuplet stores a) category, b) level and c) the message text
     */
    std::vector<std::tuple<std::string, int32_t, std::string> > messages;

    /**
     * @brief listeners is a collection of std::ostream-type objects. All recorded messages are pushed to all those objects.
     */
    std::vector<std::ostream*> listeners;

    /**
     * @brief tmp_listeners is a temporary collection of std::ostream-type objects. All recorded messages are pushed to all those objects.
     * The streams may be deleted using @ref clearTmpStreams.
     */
    std::vector<std::ostream*> tmp_listeners;

    std::mutex m;

    bool log_timestamps = false;

    std::string current_time;

    /**
     * @brief start_time is the time of construction
     */
    std::chrono::high_resolution_clock::time_point start_time;

public:
    // Compilers check accessibility before deleted status.
    // If the deleted methods were private the error message
    // would be "it's private", which may be misleading.
    // If they are public the error is "it's deleted"
    // which clearly tells the programmer to not do what
    // he is attempting to do.
    Logger(Logger const&)          = delete;
    void operator=(Logger const&)  = delete;

    void clearTmpListeners();

    /**
     * @brief log Log a message.
     * @param cat category
     * @param level verbosity level
     * @param message test of the message
     */
    void log(std::string const& cat, int32_t const level, std::string const& message);

    /**
     * @brief s_log static version of @ref log
     * @param cat
     * @param level
     * @param message
     */
    static void log_s(std::string const& cat, int32_t const level, std::string const& message);

    void printAll(std::ostream& out, int32_t max_level = std::numeric_limits<int32_t>::max()) const;

    void printCat(std::ostream &out, std::string const& cat, int32_t max_level = std::numeric_limits<int32_t>::max()) const;

    /**
     * @brief clear deletes all previously recorded messages.
     */
    void clear();

    /**
     * @brief clear_s is the static version of @ref clear.
     */
    static void clear_s();

    template<class T>
    static void print(std::ostream &out, const T &tuple);

    static void print(std::ostream &out, std::string const& cat, int32_t const level, std::string const& message);

    void addListener(std::ostream& out);
    void addTmpListener(std::ostream& out);

    void logTimestamps(bool log = true);

    /**
     * @brief getTime formats the current time as HH:MM:SS where HH = hours (0-23), MM = minutes and SS = seconds with millisecond (xx.xxxx)
     * @return
     */
    static std::string getTime();

    /**
     * @brief timeSinceStart returns the time in seconds since the construction of the singleton.
     * @return
     */
    static double timeSinceStart();
};

} // namespace cl

#endif // CATLOGGER_H
