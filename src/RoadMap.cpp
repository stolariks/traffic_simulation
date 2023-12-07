/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date  5-12-2023
 * @file RoadMap.h
 */

#include "include/RoadMap.h"

#include <algorithm>
#include <random>

Road::Road(uint32_t road_len, uint32_t max_speed) : m_max_speed(max_speed / METERS_PER_CELL), m_cell_count(road_len / METERS_PER_CELL) {
    std::random_device rd;
    m_gen = std::mt19937(rd());
    m_queue = {};
    m_road = std::vector<std::vector<std::optional<Vehicle>>>();
}

RoadMap::RoadMap(uint32_t road_len, uint32_t max_speed) : Road(road_len, max_speed) {
    m_road.emplace_back(m_cell_count, std::nullopt);
}

void RoadMap::update() {
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
            vehicle.decelerate();
        }

        // Step 2: Check driving distance
        auto driving_distance = get_driving_distance(i);
        if (vehicle.get_speed() > driving_distance) {
            vehicle.set_speed(driving_distance);
        }

        // Place the vehicle at a new position, if new position is still in scope
        uint32_t vehicle_new_pos = i + vehicle.get_speed();
        if (vehicle_new_pos < m_road[RIGHT_LANE].size()) {
            m_road[RIGHT_LANE][vehicle_new_pos] = vehicle;
        }
    }
    insert_vehicle_from_queue();
}

std::string RoadMap::to_str() const {
    std::string road_string {"||"};
    for (int32_t i = m_road[RIGHT_LANE].size() - 1; i >= 0; --i) {
        if (!m_road[RIGHT_LANE][i].has_value()) {
            road_string += ' ';
        }
        else {
            auto vehicle = m_road[RIGHT_LANE][i].value();
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
    road_string += "||";
    std::reverse(road_string.begin(), road_string.end());
    return road_string;
}

void RoadMap::insert(Vehicle vehicle) {
    for (uint8_t i = 0; i < vehicle.Length; ++i) {
        if (m_road[RIGHT_LANE][i].has_value()) {
            m_queue.push(vehicle);
            return; // Place for vehicle is already occupied
        }
    }
    m_road[RIGHT_LANE][vehicle.Length - 1] = vehicle;
}

int32_t RoadMap::get_driving_distance(int32_t from) {
    for (uint32_t i = from; i < m_road[RIGHT_LANE].size(); ++i) {
        if (m_road[RIGHT_LANE][i].has_value()) {
            return i - from - m_road[RIGHT_LANE][i].value().Length;
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
        if (m_road[RIGHT_LANE][i].has_value()){
            return false; // Cells required to place the vehicle are already occupied
        }
    }
    m_road[RIGHT_LANE][vehicle.Length - 1] = vehicle;
    m_queue.pop();
    return true;
}

uint32_t RoadMap::size() const {
    return m_road[RIGHT_LANE].size();
}

RoadMapTwoLane::RoadMapTwoLane(uint32_t road_len, uint32_t max_speed, uint8_t two_lane_portion) : Road(road_len, max_speed) {
    m_road.emplace_back(m_cell_count, std::nullopt);
    m_road.emplace_back(m_cell_count, std::nullopt);
    m_left_lane_begin = static_cast<int>((100 - two_lane_portion) / 100.f * m_cell_count);

}

void RoadMapTwoLane::update() {
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
            if (m_gen() % 100 > RAND_DEC_TH) {
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
                if (m_road[RIGHT_LANE][vehicle_new_pos].has_value()) {
                    vehicle_new_lane = LEFT_LANE; // Stay in the left lane if right lane is occupied
                }
                else {
                    vehicle_new_lane = RIGHT_LANE; // Switch lane if possible
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
                    m_gen() % 100 > RAND_OVERTAKE_TH &&
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
                m_road[vehicle_new_lane][vehicle_new_pos] = vehicle;
            }
        } // Lane update
    } // Update step
    insert_vehicle_from_queue();
}

void RoadMapTwoLane::insert(Vehicle vehicle) {
    for (uint8_t i = 0; i < vehicle.Length; ++i) {
        if (m_road[RIGHT_LANE][i].has_value()) {
            m_queue.push(vehicle);
            return; // Place for vehicle is already occupied
        }
    }
    m_road[RIGHT_LANE][vehicle.Length - 1] = vehicle;
}

int32_t RoadMapTwoLane::get_driving_distance(uint32_t from, uint8_t lane) {
    if (lane == LEFT_LANE && from < m_left_lane_begin) {
        return 0; // In case the second lane has not started yet
    }

    for (uint32_t i = from; i < m_cell_count; ++i) {
        if (m_road[lane][i].has_value()) {
            return i - from - m_road[lane][i].value().Length;
        }
    }
    return INT32_MAX;
}

bool RoadMapTwoLane::insert_vehicle_from_queue() {
    if (m_queue.empty()) {
        return false;
    }
    auto vehicle = m_queue.front();
    for(int i = 0; i < vehicle.Length; ++i) {
        if (m_road[0][i].has_value()) {
            return false;
        }
    }
    m_road[0][vehicle.Length - 1] = vehicle;
    m_queue.pop();
    return true;
}

std::string RoadMapTwoLane::to_str() const {
    std::string road_string {};
    for (int lane = 1; lane >= 0; --lane) {
        std::string lane_string {"||"};
        for (int32_t i = m_road[lane].size() - 1; i >= 0; --i) {
            if (lane == LEFT_LANE && static_cast<uint32_t>(i) < m_left_lane_begin) {
                lane_string += '#';
                continue;
            }
            if (!m_road[lane][i].has_value()) {
                lane_string += ' ';
            } else {
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
                lane_string += vehicle.to_str();
            }
        }
        lane_string += "||";
        std::reverse(lane_string.begin(), lane_string.end());
        if (lane == LEFT_LANE) {
            lane_string += '\n';
        }
        road_string += lane_string;
    }
    return road_string;
}


uint32_t RoadMapTwoLane::size() const {
    return m_cell_count;
}
