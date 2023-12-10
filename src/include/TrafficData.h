/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 9-12-2023
 * @file TrafficData.h
 */

#pragma once

#include <vector>
#include <string>

struct TrafficDataSample {
    float avg_speed;
    float density;
    float flux;
    std::vector<std::string> road_snapshot;
};

class TrafficData {
public:
    TrafficData();

    virtual
    ~TrafficData() = default;

    virtual
    std::string to_csv() const = 0;

    void add_sample(const TrafficDataSample& sample);

protected:
    /**
     * Average speed of all vehicles on the road
     */
    std::vector<float> m_avg_speed;
    /**
     * Vehicles per step
     */
    std::vector<float> m_flux;

    /**
     * Occupied cells / all cells
     */
    std::vector<float> m_traffic_density;

    /**
     * Road snapshot
     */
    std::vector<std::vector<std::string>> m_road_snapshot;
};

class OneLaneTrafficData : public TrafficData {
public:
    std::string to_csv() const override;
};

class TwoLaneTrafficData : public TrafficData {
public:
    std::string to_csv() const override;
};