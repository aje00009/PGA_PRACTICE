//
// Created by alber on 17/09/2025.
//

#include "Logger.h"

PAG::Logger* PAG::Logger::instance = nullptr;

PAG::Logger::Logger() = default;

template<typename T>
PAG::Logger& PAG::Logger::operator<<(const T& smth) {
    messages << smth;
    return *this;
}

PAG::Logger* PAG::Logger::getInstance() {
    if (!instance)
        instance = new Logger();
    return instance;
}


void PAG::Logger::clear() {
    messages.str("");
    messages.clear();
}

std::string PAG::Logger::getMessages() const {
    return this->messages.str();
}


void PAG::Logger::addMessage(const std::string& message) {
    messages << message << "\n";
}
