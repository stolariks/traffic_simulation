/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file traffic_simulator.cpp
 */


#include "include/traffic_simulation.h"
#include "include/TrafficSimulator.h"
#include "include/TrafficData.h"

#include "include/args.h"

#include <memory>


int main(int argc, char* argv[]) {
    args::ArgumentParser parser("Traffic simulation using cellular automata");
    args::HelpFlag help(parser, "help", "Display help", {'h', "help"});

    args::Group simulation_types(parser, "Simulation types", args::Group::Validators::DontCare, args::Options::Global);
    args::Command one_lane_simulator(simulation_types, "one-lane", "One lane simulation");
    args::Command two_lane_simulator(simulation_types, "two-lane", "Two lane simulation");
        args::ValueFlag<int> two_lane_portion(two_lane_simulator, "Two lane portion", "Specify the portion of two lanes in integer percentage", {"two-lane-portion"});

    args::ValueFlag<int> arrival_interval(simulation_types, "Arrival time", "Time between vehicle arrivals", {"arrival-interval"}, ARRIVAL_INTERVAL, args::Options::Global);
    args::ValueFlag<int> max_speed(simulation_types, "Max speed (m/s)", "Maximal speed in meters per second", {"max-speed"}, MAX_SPEED_MS, args::Options::Global);
    args::ValueFlag<int> road_length(simulation_types, "Road length (m)", "The length of the road section to be simulated, in meters", {'l', "road_length"}, ROAD_LENGTH_M, args::Options::Global);
    args::ValueFlag<float > time(simulation_types, "Simulation time (h)", "The length of the simulation in hours", {'t', "time"}, 1, args::Options::Global);
    args::ValueFlag<float> sim_speed_up(simulation_types, "Simulation speed up", "", {"speed-up"}, 1, args::Options::Global);
    args::Group vehicle_distribution(simulation_types, "Specify the portions of specific vehicle types. All vehicle portions have to sum up to a 1000", args::Group::Validators::AllOrNone, args::Options::Global);
        args::ValueFlag<int> car_portion(vehicle_distribution, "Portion of the cars", "", {"cars"}, CAR_PORTION_RATIO, args::Options::Global);
        args::ValueFlag<int> bus_portion(vehicle_distribution, "Portion of the buses", "", {"buses"}, BUS_PORTION_RATIO, args::Options::Global);
        args::ValueFlag<int> truck_portion(vehicle_distribution, "Portion of the trucks", "", {"trucks"}, TRUCK_PORTION_RATIO, args::Options::Global);

    try {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help&) {
        std::cout << parser;
        return EXIT_SUCCESS;
    }
    catch (args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }
    catch (args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }

    SimType simulation_type;
    if (one_lane_simulator) {
        simulation_type = SimType::OneLane;
    }
    else if (two_lane_simulator) {
        simulation_type = SimType::TwoLane;
    }

    auto simulator = std::make_unique<TrafficSimulator>(
            args::get(car_portion), // Car portion
            args::get(bus_portion), // Bus portion
            args::get(truck_portion), // Truck portion
            args::get(arrival_interval), // Arrival interval (seconds)
            args::get(max_speed), // Max speed (m/s)
            args::get(road_length), // Road length (m)
            simulation_type,
            args::get(two_lane_portion)
            );

    // Run the simulation
    auto traffic_stats = simulator->simulate(static_cast<int>(args::get(time) * HOUR_SEC), args::get(sim_speed_up));
    std::cerr << traffic_stats->to_csv();
}