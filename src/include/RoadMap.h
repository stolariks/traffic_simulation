/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file RoadMap.h
 */

#pragma once

#include "Vehicle.h"

#include <optional>
#include <vector>
#include <array>
#include <queue>
#include <string>
#include <random>

#define LEFT_LANE 1
#define RIGHT_LANE 0

class Road {
public:

    static const int METERS_PER_CELL = 5;
    static const int RAND_DEC_TH = 2;
    static const int RAND_OVERTAKE_TH = 20;

    Road(uint32_t road_len, uint32_t max_speed);

    virtual
    ~Road() = default;

    virtual
    void update() = 0;

    virtual
    void insert(Vehicle vehicle) = 0;

    virtual
    std::string to_str() const = 0;

    virtual
    uint32_t size() const = 0;

protected:

    /**
    * Take the vehicle which is in the front of the queue
    * and try to insert it into road
    * On successful insertion, pop the vehicle from queue
    * @return whether vehicle was inserted
    */
    virtual
    bool insert_vehicle_from_queue() = 0;

    uint32_t m_max_speed;
    uint32_t m_cell_count;
    std::vector<std::vector<std::optional<Vehicle>>> m_road;
    std::queue<Vehicle> m_queue;
    std::mt19937 m_gen;

};
class RoadMap : public Road{
public:
    /**
     *
     * @param cells
     * @param max_speed
     */
    RoadMap(uint32_t road_len, uint32_t max_speed);

    void update() override;

    void insert(Vehicle vehicle) override;

    std::string to_str() const override;

    uint32_t size() const override;

protected:

    bool insert_vehicle_from_queue() override;

    /**
     * Get distance to the next vehicle
     * x_lead - x - L_veh
     * @param from x
     * @return distance to the vehicle in front, int_max if there are no cars in front
     */
    int32_t get_driving_distance(int32_t from);
};

class RoadMapTwoLane : public Road {
public:
    RoadMapTwoLane(uint32_t road_len, uint32_t max_speed, uint8_t two_lane_portion);

    void update() override;

    void insert(Vehicle vehicle) override;

    std::string to_str() const override;

    uint32_t size() const override;

protected:

    bool insert_vehicle_from_queue() override;

    int32_t get_driving_distance(uint32_t from, uint8_t lane);

    uint32_t m_left_lane_begin;
};
