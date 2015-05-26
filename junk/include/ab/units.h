#pragma once

#include <cmath>
#include <vector>

#include "util/geometry.h"

namespace ab {

struct Velocity : Point {
    Velocity operator + (const Velocity to_add) const {
        Velocity v;
        v.x = x + to_add.x;
        v.y = y + to_add.y;
        return v;
    }

    Velocity Normalize (const double limit) const {
        Velocity v;
        const double abs = Abs();
        v.x = x * (limit / abs);
        v.y = y * (limit / abs);
        return v;
    }

    Vector operator * (const double time_delta) const {
        Vector v;
        v.x = x * time_delta;
        v.y = y * time_delta;
        return v;
    }

    double Abs() const {
        return sqrt(x * x + y * y);
    }
};

struct Acceleration : Point {
    Velocity operator * (const double time_delta) const {
        Velocity v;
        v.x = x * time_delta;
        v.y = y * time_delta;
        return v;
    }

    double Abs() const {
        return sqrt(x * x + y * y);
    }
};

using PlayerId = unsigned int;
using ViewerId = unsigned int;

using Score = double;

struct Player
{
    PlayerId id;
    Score score;

    Point center;
    double radius;

    Velocity velocity;
};

struct Coin
{
    Point center;
    double radius;

    Score value;
};

using FieldStateId = unsigned long long;

struct FieldState
{
    FieldStateId id;

    double radius;
    double time_delta;
    double velocity_max;

    std::vector<Player> players;

    std::vector<Coin> coins;
};

struct Turn
{
    PlayerId player_id;
    FieldStateId state_id;
    Acceleration acceleration;
};

} // namespace ab
