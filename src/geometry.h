#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <ostream>

static const double PI = 3.14159265359;
static const double PI_2 = PI * 2.0;
static const double PI_HALF = PI / 2.0;

template<typename T>
T wrap(const T& value, const T& min, const T& max)
{
    return (value < min) ? max : ((value > max) ? min : value);
};


struct Point
{
    double x{};
    double y{};

    double GetDistanceTo(const Point& dest) const
    {
        return std::sqrt(std::pow(x - dest.x, 2) + std::pow(y - dest.y, 2));
    }
};

struct Vector
{
    double x{};
    double y{};

    double GetLength() const
    {
        return std::sqrt(std::pow(x, 2) + std::pow(y, 2));
    }

    Vector operator-(const Vector& other) const
    {
        return {x - other.x, y - other.y};
    }

    Vector operator+(const Vector& other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector operator-() const
    {
        return {-x, -y};
    }

    double DotProductTo(const Vector& other) const
    {
        return x*other.x + y*other.y;
    }

    double GetAngleTo(const Vector& other) const
    {
        return std::acos(DotProductTo(other)/(GetLength()*other.GetLength()));
    }
};

std::ostream& operator<<(std::ostream& out, const Vector& value);
Vector operator*(const Vector& lhs, const double value);
Vector operator*(const double value, const Vector& rhs);

#endif // GEOMETRY_H
