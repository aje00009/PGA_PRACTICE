#include "Logger.h"

//Definition of the instance
PAG::Logger* PAG::Logger::instance = nullptr;

/**
 * @brief Default constructor (private in order to not create more objects)
 */
PAG::Logger::Logger() = default;

/**
 *
 * @tparam T Message of any type (int, string, float, etc.) to be added to the string stream
 * @param smth Message that will be added to the string stream
 * @return A reference to the Logger instance itself
 */
template<typename T>
PAG::Logger& PAG::Logger::operator<<(const T& smth) {
    messages << smth;
    return *this;
}

/**
 * @brief Gets the actual instance of create it if it is the first time is called
 * @return The instance for this class
 */
PAG::Logger* PAG::Logger::getInstance() {
    if (!instance)
        instance = new Logger();
    return instance;
}

/**
 * @brief Clears the log string stream
 */
void PAG::Logger::clear() {
    messages.str("");
    messages.clear();
}

/**
 * @brief Gets the log messages currently in the instance
 * @return All messages registered at a moment in the log
 */
std::string PAG::Logger::getMessages() const {
    return this->messages.str();
}

/**
 * @brief Method that adds a message to the current log in the instance
 * @param message Message to be added to the log
 */
void PAG::Logger::addMessage(const std::string& message) {
    messages << message << "\n";
}
