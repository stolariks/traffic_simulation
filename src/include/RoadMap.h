/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file RoadMap.h
 */

#pragma once

#include "traffic_simulation.h"
#include "Vehicle.h"
#include "TrafficData.h"

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

    static const int METERS_PER_CELL = M_PER_CELL;
    static const int RAND_DEC_TH = RAND_DECELERATION_THRESHOLD;
    static const int RAND_OVERTAKE_TH = RAND_OVERTAKE_THRESHOLD;

    Road(uint32_t road_len, uint32_t max_speed);

    virtual
    ~Road() = default;

    virtual
    TrafficDataSample update() = 0;

    void insert(Vehicle vehicle);

    virtual
    std::string to_str() const = 0;

    virtual
    uint32_t size() const = 0;

protected:

    /**
    * Take the vehicle which is in the front of the queue
    * and try to insert it into road
    * On successful insertion, pop the vehicle from queue
    * @return position of the inserted car, -1 if not inserted
    */
    int32_t insert_vehicle_from_queue();

    bool lane_free_check(uint32_t position, uint8_t lane, uint8_t vehicle_length);

    std::string to_str(uint8_t lane) const;

    std::string to_str(uint8_t lane, uint32_t lane_start) const;

    uint32_t m_max_speed;
    uint32_t m_min_speed;
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

    TrafficDataSample update() override;

    std::string to_str() const override;

    uint32_t size() const override;

protected:

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

    TrafficDataSample update() override;

    std::string to_str() const override;

    uint32_t size() const override;

protected:

    int32_t get_driving_distance(uint32_t from, uint8_t lane);

    uint32_t m_left_lane_begin;
};
