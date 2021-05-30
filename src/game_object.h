#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <math.h>
#include <functional>
#include <optional>
#include <type_traits>
#include "physic_object.h"

enum class GeneralType : std::size_t
{
    space_ship,
    rocket,
    star,
    meteor,
};

enum class SpaceShipType : std::size_t
{
    small_battle_ship,
    enemy_space_ship,
};

enum class RocketType : std::size_t
{
    base_rocket,
};

enum class StarType : std::size_t
{
    simple_star,
};

enum class MeteorType : std::size_t
{
    regular_1,
    regular_2,
};


class GameObjectType
{
public:

    template<typename T, typename std::enable_if<
             std::is_same<T, SpaceShipType>{} ||
             std::is_same<T, RocketType>{} ||
             std::is_same<T, StarType>{} ||
             std::is_same<T, MeteorType>{},
            bool>::type = true>
    GameObjectType(const GeneralType general_type, const T subtype)
    {
        general_type_ = general_type;
        subtype_ = static_cast<std::size_t>(subtype);
    }

    GameObjectType(const GameObjectType& other) :
        general_type_{other.general_type_}, subtype_{other.subtype_}
    {}

    std::tuple<GeneralType, std::size_t> Get() const
    {
        return std::make_tuple(general_type_, static_cast<std::size_t>(subtype_));
    }

    bool operator==(const GameObjectType &other) const
    {
        return (general_type_ == other.general_type_) && (subtype_ == other.subtype_);
    }

private:
    GeneralType general_type_;
    std::size_t subtype_;
};

struct GameObjectTypeHasher
{
  std::size_t operator()(const GameObjectType& object) const
  {
    const std::hash<std::size_t> hasher{};
    auto [general, sub] = object.Get();

    return ((hasher(static_cast<std::size_t>(general)) ^ (hasher(sub) << 1)) >> 1);
  }
};

class PhysicObjectWithType : public PhysicObject
{
public:
    PhysicObjectWithType(const GameObjectType object_type, const Point& position, const double heading, const Vector& velocity, const double rotation_rate) :
        PhysicObject{position, heading, velocity, rotation_rate}, object_type_{object_type}
    {
    }

    GameObjectType GetType() const {return object_type_;}

private:
    GameObjectType object_type_;
};

class Meteor : public PhysicObjectWithType
{
public:
    Meteor(MeteorType type,const Point& position, const Vector& velocity, const double rotation_change) :
        PhysicObjectWithType({GeneralType::meteor, type}, position, std::atan2(velocity.x, velocity.y), velocity, rotation_change)
    {}

    Meteor(const Meteor& other) = default;

    double GetCollisionRadius() const override {return 5.0;}

    double GetDamage() const override {return 5.0;}
    void ApplyDamage(const double damage) override {health_ -= damage;}

    bool ShouldBeDestroyed() const override {return health_ < 0.0;}

private:
    double health_{30};
};

class Rocket : public PhysicObjectWithType
{
public:
    Rocket(const Point& position, const Vector& velocity) :
        PhysicObjectWithType({GeneralType::rocket, RocketType::base_rocket}, position, std::atan2(velocity.x, velocity.y), velocity, {})
    {}

    Rocket(const Rocket& other) = default;

    double GetCollisionRadius() const override {return 5.0;}

    double GetDamage() const override {return 10.0;}
    void ApplyDamage(const double) override {is_hit_ = true;}

    bool ShouldBeDestroyed() const override {return is_hit_ || GetExistingTime() > 3.0f;}

private:
    bool is_hit_{false};
};


class SpaceShip : public PhysicObjectWithType
{
    public:
        SpaceShip() = default;
        SpaceShip(const SpaceShipType type, Point position, const double heading, double width, double length) :
            PhysicObjectWithType({GeneralType::space_ship, type}, position, heading, {}, {}),
            width_{width}, length_{length}, collision_radius_{std::sqrt(width_ * width_ +  length_ * length_)}
        {}

        std::optional<Rocket> Shoot()
        {
            if(rockets_ > 0U)
            {
                --rockets_;
                const double rocket_velocity{500.0};
                const auto cos{std::cos(heading_)};
                const auto sin{std::sin(heading_)};
                const auto safe_rad = collision_radius_ + 10.1;
                Point start_position{position_.x + safe_rad*cos, position_.y + safe_rad*sin};
                return Rocket{start_position, {velocity_.x + rocket_velocity*std::cos(heading_), velocity_.y + rocket_velocity*std::sin(heading_)}};
            }
            else return {};
        }

        void Accelerate(double acceleration, double time_delta)
        {
            if(std::fabs(acceleration) > 0.0)
            {
                fuel_ = std::max(0.0, fuel_ - time_delta*fuel_consumption_per_s_);
            }

            if(fuel_ <= 0.0)
            {
                acceleration = 0.0;
            }

            ApplyForce(acceleration*GetOrientation());
        }

        double GetCollisionRadius() const override {return collision_radius_;}

        double GetDamage() const {return 100.0;}
        void ApplyDamage(const double damage) override
        {
            health_ -= damage;
        }

        double GetHealth() const {return health_;}
        double GetRockets() const {return rockets_;}
        double GetFuel() const {return fuel_;};

        bool ShouldBeDestroyed() const override {return health_ < 0.0;}

    protected:
        const double fuel_consumption_per_s_{1.0};
        double width_{};
        double length_{};

        double health_{100.0};
        double fuel_{100.0};
        unsigned int rockets_{100U};
        double collision_radius_{};
};

class Star : public PhysicObjectWithType
{
public:
    Star(const Point& position, double gravitation, double radius) :
        PhysicObjectWithType{{GeneralType::star, StarType::simple_star}, position, {}, {}, {}},
        collision_radius_{radius}
    {
        SetGravitation(gravitation);
    }

    double GetCollisionRadius() const override {return collision_radius_;}

    double GetDamage() const override {return 10000.0; }
    void ApplyDamage(const double damage) override {};

    bool ShouldBeDestroyed() const override {return false; }

private:

    double collision_radius_{};
};


#endif // GAME_OBJECT_H
