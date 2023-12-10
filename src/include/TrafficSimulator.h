/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file TrafficSimulator.h
 */

#pragma once

#include <memory>
#include "traffic_simulation.h"
#include "RoadMap.h"
#include "Vehicle.h"
#include "TrafficData.h"

enum class SimType {
    OneLane,
    TwoLane
};

class TrafficSimulator {
public:
    TrafficSimulator(
            int car_portion,
            int bus_portion,
            int truck_portion,
            int arrival_interval,
            int max_speed_ms,
            int road_length_m,
            SimType type,
            int left_lane_portion
            );

    std::shared_ptr<TrafficData> simulate(int seconds, float speed_up_ratio);

    void reset();

private:
    int m_car_portion, m_bus_portion, m_truck_portion;
    int m_max_speed;
    int m_arrival_interval;
    std::unique_ptr<Road> m_road;
    std::mt19937 m_gen;
    std::shared_ptr<TrafficData> m_stats;
};
