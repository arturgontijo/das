#ifndef _COMMONS_UTILS_H
#define _COMMONS_UTILS_H

#include <string>
#include <chrono>
#include <map>
#include <vector>


using namespace std;

namespace commons {

class StopWatch {
    public:
        StopWatch();
        ~StopWatch();
        void start();
        void stop();
        void reset();
        unsigned long milliseconds();
        string str_time();
    private:
        bool running;
        chrono::steady_clock::time_point start_time;
        chrono::steady_clock::duration accumulator;
};

class Utils {

public:

    Utils();
    ~Utils();

    static void error(string msg);
    static void warning(string msg);
    static bool flip_coin(double true_probability = 0.5);
    static void sleep(unsigned int milliseconds = 100);
    static string get_environment(string const &key);
    static map<string, string> parse_config(string const &config_path);
    static vector<string> split(string const &str, char delimiter);
    static string join(vector<string> const &tokens, char delimiter);
};

} // namespace commons

#endif // _COMMONS_UTILS_H
