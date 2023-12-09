/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file TrafficSimulator.cpp
 */

#include "include/TrafficSimulator.h"
#include <chrono>
#include <thread>
#include <random>
#include <iostream>

TrafficSimulator::TrafficSimulator(int car_portion, int bus_portion, int truck_portion, int arrival_interval,
                                   int max_speed_ms, int road_length_m, SimType type)
                                           : m_car_portion(car_portion), m_bus_portion(bus_portion),
                                           m_truck_portion(truck_portion), m_max_speed(max_speed_ms),
                                           m_arrival_interval(arrival_interval){
    std::random_device rd;
    m_gen = std::mt19937(rd());
    switch (type) {
        case SimType::OneLane:
            m_road = std::make_unique<RoadMap>(road_length_m, m_max_speed);
        break;
        case SimType::TwoLane:
            m_road = std::make_unique<RoadMapTwoLane>(road_length_m, max_speed_ms, 100);
    }
}

void TrafficSimulator::simulate(int seconds, float speed_up_ratio = 1) {
    std::exponential_distribution<> gen_next_arrival_time(1.f / m_arrival_interval);
    std::uniform_int_distribution<int> gen_init_speed(1, 4);
    std::uniform_int_distribution<int> gen_vehicle_type(1, 1000);

    std::string road_boundary = std::string(m_road->size() + 4, '-');
    int next_arrival = gen_next_arrival_time(m_gen);
    int vehicle_type = gen_vehicle_type(m_gen);
    int initial_speed = gen_init_speed(m_gen);

    for (int i = 0; i < seconds; ++i) {
        // Insert car
        if (i == next_arrival) {
            if (vehicle_type < m_car_portion){
                // Crete a car
                m_road->insert(Vehicle(vt_t::car, initial_speed + 1));
            }
            else if (vehicle_type < m_car_portion + m_bus_portion) {
                // Crete a bus
                m_road->insert(Vehicle(vt_t::bus, initial_speed));
            }
            else {
                // Create a truck
                m_road->insert(Vehicle(vt_t::truck, initial_speed));
            }
            next_arrival = i + 1 + gen_next_arrival_time(m_gen);
            vehicle_type = gen_vehicle_type(m_gen);
        }

        std::cout << road_boundary << '\n';
        std::cout << m_road->to_str() << '\n';
        std::cout << road_boundary << '\n';
        m_road->update();

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000/speed_up_ratio)));
        std::cout << std::flush;

    }
}

void TrafficSimulator::reset() {
    m_road = std::make_unique<RoadMap>(m_road->size(), m_max_speed);
}