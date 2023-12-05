/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file traffic_simulator.cpp
 */

#include "include/OneLaneRoadSimulator.h"

#include <memory>


int main() {
    auto simulator = std::make_unique<OneLaneRoadSimulator>(
            877, // Car portion
            2, // Bus portion
            121, // Truck portion
            8, // Arrival interval (seconds)
            25, // Max speed (m/s)
            400 // Road length (m)
            );

    simulator->simulate(1000, 10);
}