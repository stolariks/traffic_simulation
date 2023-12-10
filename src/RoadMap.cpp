/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date  5-12-2023
 * @file RoadMap.h
 */

#include "include/RoadMap.h"

#include <algorithm>
#include <random>

Road::Road(uint32_t road_len, uint32_t max_speed) : m_max_speed(max_speed / METERS_PER_CELL), m_min_speed(static_cast<uint32_t>(m_max_speed * 0.4)), m_cell_count(road_len / METERS_PER_CELL) {
    std::random_device rd;
    m_gen = std::mt19937(rd());
    m_queue = {};
    m_road = std::vector<std::vector<std::optional<Vehicle>>>();
}

bool Road::lane_free_check(uint32_t position, uint8_t lane, uint8_t vehicle_length) {
    if (position >= m_road[lane].size()) {
        return false;
    }
    if (m_road[lane][position].has_value()) {
        return false;
    }
    // STEP 1: Check for car in front
    // Check road length boundaries
    if (position + 1 >= m_cell_count) {
        return false;
    }
    // Check if vehicle in front of this, by 1 has length bigger than 1
    // _______C________
    // _______BB_______
    if (m_road[lane][position + 1].has_value() && m_road[lane][position + 1].value().Length > 1) {
        return false;
    }
    // Check road length boundaries
    if (position + 2 >= m_cell_count) {
        return false;
    }
    // Check if vehicle in front of this, by 2 places has length bigger than 2
    if (m_road[lane][position + 2].has_value() && m_road[lane][position + 2].value().Length > 2) {
        return false;
    }

    // Check for a car behind (if length of this vehicle is greater than 1)
    if (vehicle_length == 1) {
        return true;
    }
    for (int i = 1; i < vehicle_length && static_cast<int>(position) - i >= 0; ++i) {
        if (m_road[lane][position - i].has_value()) {
            return false;
        }
    }
    // Now lane switch is surely possible
    return true;
}

void Road::insert(Vehicle vehicle) {
    if (vehicle.get_speed() < m_min_speed) {
        vehicle.set_speed(m_min_speed);
    }
    else if (vehicle.get_speed() > m_max_speed) {
        vehicle.set_speed(m_max_speed);
    }
    for (uint8_t i = 0; i < vehicle.Length; ++i) {
        if (!lane_free_check(i, RIGHT_LANE, vehicle.Length)) {
            m_queue.push(vehicle);
            return; // Place for vehicle is already occupied
        }
    }
    m_road[RIGHT_LANE][vehicle.Length - 1] = vehicle;
}

int32_t Road::insert_vehicle_from_queue() {
    if (m_queue.empty()) {
        return false;
    }
    auto vehicle = m_queue.front();
    for(int i = 0; i < vehicle.Length; ++i) {
        if (!lane_free_check(i, RIGHT_LANE, vehicle.Length)) {
            return -1;
        }
    }
    m_road[RIGHT_LANE][vehicle.Length - 1] = vehicle;
    m_queue.pop();
    return vehicle.Length - 1;
}

std::string Road::to_str(uint8_t lane) const {
    return Road::to_str(lane, 0);
}

std::string Road::to_str(uint8_t lane, uint32_t lane_start) const {
    std::string road_string {""};
    for (int32_t i = m_road[lane].size() - 1; i >= 0; --i) {
        if (static_cast<uint32_t>(i) < lane_start) {
            road_string += '#';
            continue;
        }
        if (!m_road[lane][i].has_value()) {
            road_string += '.';
        }
        else {
            auto vehicle = m_road[lane][i].value();
            switch (vehicle.get_vehicle_type()) {
                case vt_t::car:
                    break;
                case vt_t::bus:
                    i -= 1;
                    break;
                case vt_t::truck:
                    i -= 2;
                    break;
            }
            road_string += vehicle.to_str();
        }
    }
    std::reverse(road_string.begin(), road_string.end());
    return road_string;
}

RoadMap::RoadMap(uint32_t road_len, uint32_t max_speed) : Road(road_len, max_speed) {
    m_road.emplace_back(m_cell_count, std::nullopt);
}

TrafficDataSample RoadMap::update() {
    TrafficDataSample stats {};
    uint32_t num_vehicles = 0;
    uint32_t num_occupied_spaces = 0;

    for (int32_t i = m_road[RIGHT_LANE].size() - 1; i >= 0; --i) {
        if (!m_road[RIGHT_LANE][i].has_value()) {
            continue;
        }
        // Take vehicle of the road
        auto vehicle = *(m_road[RIGHT_LANE][i]);
        m_road[RIGHT_LANE][i].reset();

        // Step 1: Random acceleration / deceleration
        if (m_gen() % 100 > RAND_DEC_TH) {
            if (vehicle.get_speed() < m_max_speed) {
                vehicle.accelerate();
            }
        }
        else {
            if (vehicle.get_speed() >= m_min_speed) {
                vehicle.decelerate();
            }
        }

        // Step 2: Check driving distance
        auto driving_distance = get_driving_distance(i);
        if (vehicle.get_speed() > driving_distance) {
            vehicle.set_speed(driving_distance);
        }

        // Step 3: Place the vehicle at a new position, if new position is still in scope
        uint32_t vehicle_new_pos = i + vehicle.get_speed();
        if (vehicle_new_pos < m_road[RIGHT_LANE].size()) {
            // Collect data about the vehicle
            num_vehicles += 1;
            num_occupied_spaces += vehicle.Length;
            stats.avg_speed += vehicle.get_speed();

            m_road[RIGHT_LANE][vehicle_new_pos] = vehicle;
        }
        else {
            // Vehicle left the road in the current time step
            stats.flux += 1;
        }
    }
    auto new_vehicle_pos = insert_vehicle_from_queue();
    if (new_vehicle_pos >= 0) {
        if (m_road[RIGHT_LANE][new_vehicle_pos].has_value()){
            auto vehicle = m_road[RIGHT_LANE][new_vehicle_pos].value();
            num_vehicles++;
            num_occupied_spaces += vehicle.Length;
            stats.avg_speed += vehicle.get_speed();
        }
    }
    stats.avg_speed = num_vehicles > 0 ? stats.avg_speed / num_vehicles : 0;
    stats.density = num_occupied_spaces / static_cast<float>(m_road[RIGHT_LANE].size());
    stats.road_snapshot = {this->to_str()};
    return stats;
}

std::string RoadMap::to_str() const {
    return Road::to_str(RIGHT_LANE);
}

int32_t RoadMap::get_driving_distance(int32_t from) {
    for (uint32_t i = from; i < m_road[RIGHT_LANE].size(); ++i) {
        if (m_road[RIGHT_LANE][i].has_value()) {
            return i - from - m_road[RIGHT_LANE][i].value().Length;
        }
    }
    return INT32_MAX;
}

uint32_t RoadMap::size() const {
    return m_road[RIGHT_LANE].size();
}

RoadMapTwoLane::RoadMapTwoLane(uint32_t road_len, uint32_t max_speed, uint8_t two_lane_portion) : Road(road_len, max_speed) {
    m_road.emplace_back(m_cell_count, std::nullopt);
    m_road.emplace_back(m_cell_count, std::nullopt);
    m_left_lane_begin = static_cast<int>((100 - two_lane_portion) / 100.f * m_cell_count);

}

TrafficDataSample RoadMapTwoLane::update() {
    TrafficDataSample stats {};
    uint32_t num_vehicles = 0;
    uint32_t num_occupied_spaces = 0;

    uint32_t vehicle_new_pos;
    uint8_t vehicle_new_lane;

    for (int32_t x = m_cell_count - 1; x >= 0; --x) {
        for (int8_t l = 1; l >= 0; --l){
            if (!m_road[l][x].has_value()) {
                continue;
            }
            // Take vehicle of the road and alter it
            auto vehicle = *(m_road[l][x]);
            m_road[l][x].reset();

            // Step 1: Random acceleration / deceleration
            if (l == LEFT_LANE || vehicle.get_speed() <= m_min_speed || m_gen() % 100 > RAND_DEC_TH) {
                if (vehicle.get_speed() < m_max_speed) {
                    vehicle.accelerate();
                }
            }
            else {
                vehicle.decelerate();
            }

            // Step 2: Check driving distances and overtaking opportunities
            if (l == LEFT_LANE) { // Left lane code
                auto driving_distance_left = get_driving_distance(x, LEFT_LANE);
                if (vehicle.get_speed() > driving_distance_left) {
                    vehicle.set_speed(driving_distance_left);
                }
                vehicle_new_pos = x + vehicle.get_speed();
                if (lane_free_check(vehicle_new_pos, RIGHT_LANE, vehicle.Length)) {
                    vehicle_new_lane = RIGHT_LANE; // Switch lane if possible
                }
                else {
                    vehicle_new_lane = LEFT_LANE; // Stay in the left lane if right lane is occupied
                }
            } // Left lane

            else { // Right lane code
                // Lane switch condition
                // If left lane started
                // If faster than vehicle in front
                // If overtake wanted
                // If the distance in the left lane is bigger than the distance in right lane
                if (static_cast<uint32_t>(x) > m_left_lane_begin &&
                    vehicle.get_speed() > get_driving_distance(x, RIGHT_LANE) &&
                    m_gen() % 100 < RAND_OVERTAKE_TH &&
                    get_driving_distance(x, LEFT_LANE) > get_driving_distance(x, RIGHT_LANE)
                                ) { // Overtake
                    vehicle_new_lane = LEFT_LANE;
                }
                else { // Don't overtake
                    vehicle_new_lane = RIGHT_LANE;
                }
                // Check driving distance
                if (vehicle.get_speed() > get_driving_distance(x, vehicle_new_lane)) {
                    vehicle.set_speed(get_driving_distance(x, vehicle_new_lane));
                }
                vehicle_new_pos = x + vehicle.get_speed();
            }

            // Step 3: Place the vehicle
            if (vehicle_new_pos < m_cell_count) {
                num_vehicles += 1;
                num_occupied_spaces += vehicle.Length;
                stats.avg_speed += vehicle.get_speed();
                m_road[vehicle_new_lane][vehicle_new_pos] = vehicle;
            }
            else {
                stats.flux += 1;
            }
        } // Lane update
    } // Update step
    auto new_vehicle_pos = insert_vehicle_from_queue();
    if (new_vehicle_pos >= 0) {
        if (m_road[RIGHT_LANE][new_vehicle_pos].has_value()){
            auto vehicle = m_road[RIGHT_LANE][new_vehicle_pos].value();
            num_vehicles++;
            num_occupied_spaces += vehicle.Length;
            stats.avg_speed += vehicle.get_speed();
        }
    }
    stats.avg_speed = num_vehicles > 0 ? stats.avg_speed / num_vehicles : 0;
    stats.density = num_occupied_spaces / static_cast<float>(m_road[RIGHT_LANE].size() + m_road[LEFT_LANE].size());
    stats.road_snapshot = {Road::to_str(RIGHT_LANE), Road::to_str(LEFT_LANE, m_left_lane_begin)};
    return stats;
}

int32_t RoadMapTwoLane::get_driving_distance(uint32_t from, uint8_t lane) {
    if (lane == LEFT_LANE && from < m_left_lane_begin) {
        return 0; // In case the second lane has not started yet
    }

    for (uint32_t i = from; i < m_cell_count; ++i) {
        if (m_road[lane][i].has_value()) {
            int32_t distance = i - from - m_road[lane][i].value().Length;
            return distance >= 0 ? distance : 0;
        }
    }
    return INT32_MAX;
}

std::string RoadMapTwoLane::to_str() const {
    std::string road_string {};
    road_string += Road::to_str(LEFT_LANE, m_left_lane_begin) + '\n';
    road_string += Road::to_str(RIGHT_LANE);
    return road_string;
}


uint32_t RoadMapTwoLane::size() const {
    return m_cell_count;
}
