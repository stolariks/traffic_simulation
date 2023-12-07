/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file Vehicle.cpp
 */

#include "include/Vehicle.h"

#include <cmath>

Vehicle::Vehicle(vt_t vehicle_type, uint8_t initial_speed) : m_current_speed(initial_speed), m_type(vehicle_type) {
    switch (m_type) {
        case vt_t::car:
            Acceleration = 0.14;
            Deceleration = 0.52;
            Length = 1;
            break;
        case vt_t::bus:
            Acceleration = 0.05;
            Deceleration = 0.1;
            Length = 2;
            break;
        case vt_t::truck:
            Acceleration = 0.04;
            Deceleration = 0.09;
            Length = 3;
            break;
    }
}

uint8_t Vehicle::get_speed() const {
    return std::floor(m_current_speed);
}

void Vehicle::set_speed(uint8_t speed) {
    m_current_speed = speed;
}

void Vehicle::accelerate() {
    m_current_speed += Acceleration;
}

void Vehicle::decelerate() {
    if (m_current_speed - Deceleration > 0){
        m_current_speed -= Deceleration;
    }
}

std::string Vehicle::to_str() const {
    std::string veh_string {};
    switch (m_type) {
        case vt_t::car:
            veh_string = std::to_string(this->get_speed());
            break;
        case vt_t::bus:
            veh_string = std::to_string(this->get_speed()) + "B";
            break;
        case vt_t::truck:
            veh_string = std::to_string(this->get_speed()) + "TT";
            break;
    }

    return veh_string;
}

vt_t Vehicle::get_vehicle_type() const {
    return m_type;
}
