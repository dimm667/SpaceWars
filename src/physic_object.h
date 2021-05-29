#ifndef PHYSIC_OBJECT_H
#define PHYSIC_OBJECT_H

#include "geometry.h"

class PhysicObject
{
public:
        PhysicObject(const Point& position, const double heading, const Vector& velocity, double rotation_rate);

        void ApplyForce(const Vector& force);

        void AddForces(const Vector& force);

        void ApplyRotation(const double rotation_rate_change);

        void UpdateDynamicState(double time_delta);

        void SetGravitation(const double gravitation);

        Vector GetGravitationAt(const Point& point);

        const Point& GetPosition() const;
        const Vector GetOrientation() const;
        const Vector& GetVelocity() const;
        const double& GetHeading() const;
        const double& GetExistingTime() const;
        virtual double GetCollisionRadius() const = 0;
        virtual double GetDamage() const = 0;
        virtual void ApplyDamage(const double damage) = 0;
        virtual bool ShouldBeDestroyed() const = 0;
        virtual ~PhysicObject() = default;

protected:
    double gravitation_{};
    Point position_{};
    double heading_{};
    Vector velocity_{};
    Vector acceleration_{};
    double rotation_rate_change_{};
    double existing_time_{};
};

#endif // PHYSIC_OBJECT_H
