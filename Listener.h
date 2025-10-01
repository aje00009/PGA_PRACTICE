//
// Created by alber on 25/09/2025.
//

#ifndef PGA_PRACTICA_LISTENER_H
#define PGA_PRACTICA_LISTENER_H

//Enum class for every type of window GUI draws
enum class WindowType {
    BackGround,
    Controls,
    Messages
};

/**
 * @brief Interface whose classes which implement such interface wakes all users (objects) that have suscribed to an event
 */
namespace PAG {
    class Listener {
    public:
        Listener() = default;
        virtual ~Listener() = default;
        virtual void wakeUp(WindowType t, ... ) = 0;

    };
}

#endif //PGA_PRACTICA_LISTENER_H