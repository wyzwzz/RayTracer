//
// Created by wyz on 2021/3/10.
//

#ifndef RAYTRACER_UTIL_H
#define RAYTRACER_UTIL_H
#include <chrono>
#include <spdlog/spdlog.h>
#define START_TIMER auto _start = std::chrono::steady_clock::now();

#define END_TIMER(fmt)                                                                                                 \
    auto _end = std::chrono::steady_clock::now();                                                                       \
    auto _t = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start);                                       \
    spdlog::info("{0} cost time {1:d} ms",fmt,_t.count());




#endif //RAYTRACER_UTIL_H
