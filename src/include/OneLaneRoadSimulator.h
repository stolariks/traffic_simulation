/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file OneLaneRoadSimulator.h
 */

#pragma once

#include <memory>
#include "RoadMap.h"
#include "Vehicle.h"

class OneLaneRoadSimulator {
public:
    OneLaneRoadSimulator(
            int car_portion,
            int bus_portion,
            int truck_portion,
            int arrival_interval,
            int max_speed_ms,
            int road_length_m
            );

    static
    const int CAR_PORTION = 877;

    static
    const int BUS_PORTION = 2;

    static
    const int TRUCK_PORTION = 121;

    void simulate(int seconds, float speed_up_ratio);

    void reset();

private:
    int m_car_portion, m_bus_portion, m_truck_portion;
    int m_max_speed;
    int m_arrival_interval;
    std::unique_ptr<RoadMap> m_road;
};
