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

#endif /* UID_GENERATOR_H */