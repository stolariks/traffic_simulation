/**
 * @authors Samuel Stolarik, Jan Pavlicek
 * @date 5-12-2023
 * @file Vehicle.h
 */

#pragma once

#include <cstdint>
#include <string>

enum class vt_t {
    car,
    bus,
    truck
};


class Vehicle {
public:
    float Acceleration;

    float Deceleration;

    uint8_t Length;

    explicit Vehicle(vt_t vehicle_type, uint8_t initial_speed = 1);

    virtual
    ~Vehicle() = default;

    /**
     * Get speed of vehicle in cells per second
     * @return speed of the vehicle rounded to integer
     */
    uint8_t get_speed() const;

    void set_speed(uint8_t speed);

    vt_t get_vehicle_type() const;

    void accelerate();

    void decelerate();

    std::string to_str() const;

protected:
    float m_current_speed;
    vt_t m_type;
};
