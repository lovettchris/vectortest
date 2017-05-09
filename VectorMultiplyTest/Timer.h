#pragma once

#include <chrono>
class Timer {
public:
    Timer() {

    }
    void start() {
        start_ = now();
    }
    void stop() {
        end_ = now();
    }
    double seconds() {
        return (end_ - start_) / 1000000000.0;
    }
    double milliseconds() {
        return (end_ - start_) / 1000000.0;
    }
    double microseconds() {
        return (end_ - start_) / 1000.0;
    }
    double nanoseconds() {
        return (end_ - start_);
    }
private:
    double now() {
        return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
    }
    double start_;
    double end_;
};
