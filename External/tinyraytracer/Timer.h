//
// Created by ljgdsq on 2023/7/6.
//

#pragma once

#include <chrono>


class Timer {
public:
    Timer() {
        _time = std::chrono::system_clock::now();
    }

    double GetElapseTime() {
        auto endTime = std::chrono::system_clock::now();
        return std::chrono::duration<double>(endTime - _time).count() * 1000;
    }

private:

    std::chrono::time_point<std::chrono::system_clock> _time;
};
