//
// Created by alber on 17/09/2025.
//

#ifndef PGA_PRACTICA_LOGGER_H
#define PGA_PRACTICA_LOGGER_H

#include <sstream>

/**
 * @brief Class that stores the messages for logger window (errors, info...)
 */
namespace PAG {
    class Logger {
    private:
        static Logger* instance;
        std::stringstream messages;

        Logger();
    public:
        template<class T>
        Logger& operator<<(const T& smth);

        void addMessage(const std::string& message);
        static Logger* getInstance();
        void clear();
        std::string getMessages() const;
    };
}


#endif //PGA_PRACTICA_LOGGER_H