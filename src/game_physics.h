#ifndef GAME_PHYSICS_H
#define GAME_PHYSICS_H

#include <list>

class GamePhysicEmulator
{
public:
    GamePhysicEmulator() = default;

    template<class T>
    void Emulate(std::list<T>& objects, const double time_delta)
    {
        for(auto& object_1 : objects)
        {
            for(auto& object_2 : objects)
            {
                if(object_1 != object_2)
                {
                    object_1->AddForces(object_2->GetGravitationAt(object_1->GetPosition()));
                }
            }
        }

        for(auto& object : objects)
        {
            object->UpdateDynamicState(time_delta);
        }

        for(auto obj1_it{objects.begin()}; obj1_it != objects.end(); ++obj1_it)
        {
            for(auto obj2_it{std::next(obj1_it, 1)}; obj2_it != objects.end(); ++obj2_it)
            {
                CheckCollision(**obj1_it, **obj2_it);
            }
        }

    }

private:

    template <class T, class Q>
    void CheckCollision(T& object1, Q& object2)
    {
        if(object1.GetPosition().GetDistanceTo(object2.GetPosition()) < (object1.GetCollisionRadius() + object2.GetCollisionRadius()))
        {
            object1.ApplyDamage(object2.GetDamage());
            object2.ApplyDamage(object1.GetDamage());
        }
    }
};

#endif // GAME_PHYSICS_H
