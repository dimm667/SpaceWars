#include "physic_object.h"

PhysicObject::PhysicObject(const Point& position, const double heading, const Vector& velocity, double rotation_rate) :
    position_{position}, velocity_{velocity}, heading_{heading}, rotation_rate_change_{rotation_rate}
{}

void PhysicObject::ApplyForce(const Vector& force)
{
    acceleration_ = force;
}

void PhysicObject::AddForces(const Vector& force)
{
    acceleration_ = acceleration_ + force;
}

void PhysicObject::ApplyRotation(const double rotation_rate_change)
{
    rotation_rate_change_ = rotation_rate_change;
}

void PhysicObject::UpdateDynamicState(double time_delta)
{
    existing_time_ += time_delta;
    velocity_.x += acceleration_.x * time_delta;
    velocity_.y += acceleration_.y * time_delta;

    position_.x += velocity_.x * time_delta;
    position_.y += velocity_.y * time_delta;

    position_.x = wrap(position_.x, 0.0, 1920.0);
    position_.y = wrap(position_.y, 0.0, 1080.0);

    heading_ += rotation_rate_change_ * time_delta;
    heading_ = wrap(heading_, -PI, PI);

    acceleration_ = {};
}

void PhysicObject::SetGravitation(const double gravitation)
{
    gravitation_ = gravitation;
}

Vector PhysicObject::GetGravitationAt(const Point& point)
{
    const double distance_square = std::max(std::pow(position_.GetDistanceTo(point), 2), 0.01);
    const double gravitation{ gravitation_ / distance_square};
    const double force_angle = std::atan2(position_.x - point.x, position_.y - point.y);

    return {gravitation * std::sin(force_angle), gravitation * std::cos(force_angle)};
}

const Point& PhysicObject::GetPosition() const {return position_;}
const Vector PhysicObject::GetOrientation() const {return {std::cos(heading_), std::sin(heading_)};}
const Vector& PhysicObject::GetVelocity() const {return velocity_;}
const double& PhysicObject::GetHeading() const {return heading_;}
const double& PhysicObject::GetExistingTime() const {return existing_time_;}
