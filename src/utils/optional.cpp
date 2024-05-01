#include <stdexcept>
#include "optional.hpp"

/**
 * @brief Default constructor.
 *
 * Constructs an empty Optional object with no value set.
 */
template <typename T>
Optional<T>::Optional() : is_set(false) {}

/**
 * @brief Constructor with value initialization.
 *
 * Constructs an Optional object with the provided value set.
 *
 * @param val The value to be set.
 */
template <typename T>
Optional<T>::Optional(const T &val) : is_set(true), val(val)
{
}

/**
 * @brief Checks if a value is set.
 *
 * @return true if a value is set, false otherwise.
 */
template <typename T>
bool Optional<T>::has_value() const
{
    return is_set;
}

/**
 * @brief Retrieves the value.
 *
 * If a value is set, returns a const reference to the stored value.
 * If no value is set, throws a logic_error exception.
 *
 * @return const reference to the stored value.
 * @throws std::logic_error if no value is set.
 */
template <typename T>
const T &Optional<T>::value() const
{
    if (!is_set)
    {
        throw std::logic_error("Optional value not set");
    }
    return val;
}

/**
 * @brief Retrieves the value or a default value.
 *
 * If a value is set, returns a const reference to the stored value.
 * If no value is set, returns the provided default value.
 *
 * @param default_value The value to return if no value is set.
 * @return const reference to the stored value or the default value.
 */
template <typename T>
const T &Optional<T>::value_or(const T &default_value) const
{
    return is_set ? val : default_value;
}