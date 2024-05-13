#ifndef UID_GENERATOR_H
#define UID_GENERATOR_H

#include <string>

/**
 * @brief Generate a unique ID of the specified size.
 *
 * @param size The size of the generated ID.
 * @return std::string The generated unique ID.
 */
std::string generate_uid(int size);

/**
 * @brief Generate a unique ID based on the current date and time.
 *
 * @return std::string The generated unique ID.
 */
std::string generate_date_uid();

#endif /* UID_GENERATOR_H */