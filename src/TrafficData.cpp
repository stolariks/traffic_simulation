/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 9-12-2023
 * @file TrafficData.cpp
 */

#include "include/TrafficData.h"
#include "include/RoadMap.h"

TrafficData::TrafficData() {
    m_avg_speed = std::vector<float>();
    m_flux = std::vector<float>();
    m_traffic_density = std::vector<float>();

    m_road_snapshot = std::vector<std::vector<std::string>>();
}

void TrafficData::add_sample(const TrafficDataSample &sample) {
    m_avg_speed.push_back(sample.avg_speed);
    m_flux.push_back(sample.flux);
    m_traffic_density.push_back(sample.density);
    m_road_snapshot.push_back(sample.road_snapshot);
}

std::string OneLaneTrafficData::to_csv() const {
    static const std::string delim {";"};
    std::string csv {"avg_speed" + delim + "density" + delim + "flux" + delim + "lane_right" + '\n'};

    for (uint32_t i = 0; i < m_avg_speed.size(); ++i) {
        csv += std::to_string(m_avg_speed[i]);
        csv += delim;
        csv += std::to_string(m_traffic_density[i]);
        csv += delim;
        csv += std::to_string(m_flux[i]);
        csv += delim;
        csv += m_road_snapshot[i][RIGHT_LANE];
        csv += delim + '\n';
    }

    return csv;
}


std::string TwoLaneTrafficData::to_csv() const {
    static const std::string delim {";"};
    std::string csv {"avg_speed" + delim + "density" + delim + "flux" + delim + "lane_right" + delim + "lane_left" + '\n'};

    for (uint32_t i = 0; i < m_avg_speed.size(); ++i) {
        csv += std::to_string(m_avg_speed[i]);
        csv += delim;
        csv += std::to_string(m_traffic_density[i]);
        csv += delim;
        csv += std::to_string(m_flux[i]);
        csv += delim;
        csv += m_road_snapshot[i][RIGHT_LANE];
        csv += delim;
        csv += m_road_snapshot[i][LEFT_LANE];
        csv += delim;
        csv +='\n';
    }
    return csv;
}

