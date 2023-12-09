/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file traffic_simulator.cpp
 */


#include "include/traffic_simulation.h"
#include "include/TrafficSimulator.h"

#include <memory>


int main() {
    auto simulator = std::make_unique<TrafficSimulator>(
            CAR_PORTION_RATIO, // Car portion
            BUS_PORTION_RATIO, // Bus portion
            TRUCK_PORTION_RATIO, // Truck portion
            ARRIVAL_INTERVAL, // Arrival interval (seconds)
            MAX_SPEED_MS, // Max speed (m/s)
            ROAD_LENGTH_M, // Road length (m)
            SimType::OneLane
            );

    simulator->simulate(1 * HOUR_SEC, 100);
}