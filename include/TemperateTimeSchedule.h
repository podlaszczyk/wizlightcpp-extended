#pragma once

#include <string>
#include <chrono>

struct TemperatureTimeFrame
{
    int startTemperature;
    int endTemperature;

    std::tm startTime;
    std::tm endTime;
};
struct TemperatureTimeSchedule{
    std::string m_devIP;
    std::string name;
    std::vector<TemperatureTimeFrame> temperatureTimeFrames;
};
