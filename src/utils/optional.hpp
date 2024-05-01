#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <stdexcept>

/**
 * @brief A simple optional-like class template.
 *
 * This class provides a basic optional-like functionality, allowing you to
 * store a value that may or may not be present.
 *
 * @tparam T The type of the optional value.
 */
template <typename T>
class Optional
{
private:
    bool is_set; // Flag indicating whether the value is set.
    T val;       // The optional value.

public:
    /**
     * @brief Default constructor.
     *
     * Constructs an empty Optional object with no value set.
     */
    Optional();

    /**
     * @brief Constructor with value initialization.
     *
     * Constructs an Optional object with the provided value set.
     *
     * @param val The value to be set.
     */
    Optional(const T &val);

    /**
     * @brief Checks if a value is set.
     *
     * @return true if a value is set, false otherwise.
     */
    bool has_value() const;

    /**
     * @brief Retrieves the value.
     *
     * If a value is set, returns a const reference to the stored value.
     * If no value is set, throws a logic_error exception.
     *
     * @return const reference to the stored value.
     * @throws std::logic_error if no value is set.
     */
    const T &value() const;

    /**
     * @brief Retrieves the value or a default value.
     *
     * If a value is set, returns a const reference to the stored value.
     * If no value is set, returns the provided default value.
     *
     * @param default_value The value to return if no value is set.
     * @return const reference to the stored value or the default value.
     */
    const T &value_or(const T &default_value) const;
};

#endif // OPTIONAL_H
