#include "geometry.h"

std::ostream& operator<<(std::ostream& out, const Vector& value)
{
   return out << "[" << value.x << ", " << value.y << "]";
}

Vector operator*(const Vector& lhs, const double value)
{
    return {lhs.x * value, lhs.y * value};
}

Vector operator*(const double value, const Vector& rhs)
{
    return rhs * value;
}
