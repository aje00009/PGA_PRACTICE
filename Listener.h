//
// Created by alber on 25/09/2025.
//

#ifndef PGA_PRACTICA_LISTENER_H
#define PGA_PRACTICA_LISTENER_H

enum class WindowType {
    BackGround,
    Controls,
    Messages
};

namespace PAG {
    class Listener {
    public:
        Listener() = default;
        virtual ~Listener() = default;
        virtual void wakeUp(WindowType t, ... ) = 0;

    };
}

#endif //PGA_PRACTICA_LISTENER_H