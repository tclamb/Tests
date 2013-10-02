#include <chrono>
#include <thread>
#include <atomic>
#include <iostream>
#include <iomanip>

#include <ctime>

/* Signal Handling Includes */
#include <cstdio>
#include <cstdlib>
#include <csignal>

/* Platform Specific Includes */
#ifdef WIN32
#include <windows.h>
#else
#include <termios.h>
#endif

std::atomic<bool> lap{false};
bool die = false;

/* ANSI Escape Sequences */
const char* to_first_column = "\x1b[G";
const char* hide_cursor     = "\x1b[?25l";
const char* show_cursor     = "\x1b[?25h";

/* Signal Handler */
void cleanup(int) { die = true; };

/* Platform Specific Terminal Setup */
#ifdef WIN32
HANDLE hStdin;
DWORD mode = 0;
void setup_terminal() {
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT
                                & ENABLE_PROCESSED_INPUT);
}
void reset_terminal() {
    SetConsoleMode(hStdin, mode);
}
#else
struct termios original_settings;
void setup_terminal() {
    tcgetattr(0, &original_settings);

    struct termios settings = original_settings;
    settings.c_lflag &= ~ICANON;
    settings.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &settings);
}
void reset_terminal() {
    tcsetattr(0, TCSANOW, &original_settings);
}
#endif

void countdown() {
    /* Setup Terminal */
    std::cout << hide_cursor << std::setfill('0');
    setup_terminal();

    using jiffies = std::chrono::duration<int, std::centi>;
    auto now_time = std::time(NULL);
    auto end_time = std::localtime(&now_time);
    end_time->tm_sec = 0; end_time->tm_min = 0; end_time->tm_hour = 18;
    auto end_time_t = std::mktime(end_time);
    auto end = std::chrono::system_clock::from_time_t(end_time_t);

    bool prefix = false;

    while(!die) {
        auto delta = std::chrono::duration_cast<jiffies>(
            end - std::chrono::system_clock::now()
            ).count();

        if(delta < 0) {
            prefix = true;
            delta *= -1;
        }

        auto cs = delta%100;
        auto s = delta/100;
        auto m = s/60; s %= 60;
        auto h = m/60; m %= 60;

        std::cout << to_first_column
            << (prefix ? "-" : "")
            << std::setw(2) << h << ':'
            << std::setw(2) << m << ':'
            << std::setw(2) << s << '.'
            << std::setw(2) << cs
            << std::flush;

        if(lap.exchange(false)) {
            std::cout << std::endl;
        }
        
        /* justification: something about Nyquist frequencies, etc. */
        std::this_thread::sleep_for(std::chrono::milliseconds{4});
    }

    /* Reset Terminal */
    reset_terminal();
    std::cout << show_cursor << std::endl;
    std::exit(0);
}


int main() {
    /* Handle Signals */
    std::signal(SIGTERM, cleanup);
    std::signal(SIGINT , cleanup);
    std::signal(SIGABRT, cleanup);

    std::thread sw{countdown};

    while(true) {
        if(std::getchar() != '\x03') /* ctrl+c */
            lap.store(true);
    }

    return 0;
}
