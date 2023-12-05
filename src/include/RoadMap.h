/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file RoadMap.h
 */

#pragma once

#include "Vehicle.h"

#include <optional>
#include <vector>
#include <queue>
#include <string>
#include <random>

class RoadMap {
public:
    static const int METERS_PER_CELL = 5;
    static const int RAND_DEC_TH = 2;

    /**
     *
     * @param cells
     * @param max_speed
     */
    explicit RoadMap(uint32_t road_len, int32_t max_speed);

    void update();

    void insert(Vehicle vehicle);

    std::string to_str() const;

    uint32_t size() const;

private:

    /**
     * Take the vehicle which is in the front of the queue
     * and try to insert it into road
     * On successful insertion, pop the vehicle from queue
     * @return whether vehicle was inserted
     */
    bool insert_vehicle_from_queue();

    /**
     * Get distance to the next vehicle
     * x_lead - x - L_veh
     * @param from x
     * @return distance to the vehicle in front, int_max if there are no cars in front
     */
    int32_t get_driving_distance(int32_t from);
    uint32_t m_max_speed;
    uint32_t m_cell_count;
    std::queue<Vehicle> m_queue;
    std::vector<std::optional<Vehicle>> m_road;
    std::mt19937 m_gen;

};
