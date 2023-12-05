/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date  5-12-2023
 * @file RoadMap.h
 */

#include "include/RoadMap.h"

#include <random>

RoadMap::RoadMap(uint32_t road_len, int32_t max_speed) : m_max_speed(max_speed / METERS_PER_CELL), m_cell_count(road_len / METERS_PER_CELL) {
    m_queue = {};
    m_road = std::vector<std::optional<Vehicle>>(m_cell_count, std::nullopt); // Create an empty road
    std::random_device rd;
    m_gen = std::mt19937(rd());
}

void RoadMap::update() {
    for (int32_t i = m_road.size() - 1; i >= 0; --i) {
        if (!m_road[i].has_value()) {
            continue;
        }
        // Take vehicle of the road
        auto vehicle = *(m_road[i]);
        m_road[i].reset();

        // Step 1: Random acceleration / deceleration
        if (m_gen() % 100 > RAND_DEC_TH) {
            if (vehicle.get_speed() < m_max_speed) {
                vehicle.accelerate();
            }
        }
        else {
            vehicle.decelerate();
        }

        // Step 2: Check driving distance
        auto driving_distance = get_driving_distance(i);
        if (vehicle.get_speed() > driving_distance) {
            vehicle.set_speed(driving_distance);
        }

        // Place the vehicle at a new position, if new position is still in scope
        uint32_t vehicle_new_pos = i + vehicle.get_speed();
        if (vehicle_new_pos < m_road.size()) {
            m_road[vehicle_new_pos] = vehicle;
        }
    }
    insert_vehicle_from_queue();
}

std::string RoadMap::to_str() const {
    std::string road_string {};
    for (const auto& road_cell : m_road) {
        if (!road_cell.has_value()) {
            road_string += '-';
        }
        else {
            road_string += road_cell.value().to_str();
        }
    }
    return road_string;
}

void RoadMap::insert(Vehicle vehicle) {
    for (int i = 0; i < vehicle.Length; ++i) {
        if (m_road[i].has_value()) {
            m_queue.push(vehicle);
            return; // Place for vehicle is already occupied
        }
    }
    m_road[vehicle.Length - 1] = vehicle;
}

int32_t RoadMap::get_driving_distance(int32_t from) {
    for (uint32_t i = from; i < m_road.size(); ++i) {
        if (m_road[i].has_value()) {
            return i - from - m_road[i].value().Length;
        }
    }
    return INT32_MAX;
}

bool RoadMap::insert_vehicle_from_queue() {
    if (m_queue.empty()){
        return false;
    }
    auto vehicle = m_queue.front();
    for (int i = 0; i < vehicle.Length; ++i) {
        if (m_road[i].has_value()){
            return false; // Cells required to place the vehicle are already occupied
        }
    }
    m_road[vehicle.Length - 1] = vehicle;
    m_queue.pop();
    return true;
}

uint32_t RoadMap::size() const {
    return m_road.size();
}
