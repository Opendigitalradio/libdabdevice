#ifndef DABDEVICE_TYPES_GAIN
#define DABDEVICE_TYPES_GAIN

#include <cstdint>

#include <ostream>

namespace dab
  {

  /**
   * @author Felix Morgner
   *
   * @brief A type to represent gains in decibel
   *
   * This type is a simple wrapper for a float. It allows to specify that a gain is required.
   * There exist UDLs to create gains in dB in the namespace dab::literals.
   */
  struct gain
    {
    /**
     * @brief Construct a gain representing the supplied number of decibels.
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    explicit constexpr gain(float const decibel)
      : m_value{decibel}
      {

      }

    /**
     * @brief Convert a gain to float for APIs that do not understand dab::gain, like librtlsdr.
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    explicit constexpr operator float() const
      {
      return m_value;
      }

    /**
     * @brief Get the underlying decibel value in its native type
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    float value() const
      {
      return static_cast<float>(*this);
      }

    private:
      float m_value;
    };

  /**
   * @brief Write a gain to a stream
   *
   * @author Felix Morgner
   * @since  1.0.1
   */
  std::ostream & operator<<(std::ostream & out, gain const & gain)
    {
    return out << gain.value() << " dB";
    }

  /**
   * @namespace literals
   *
   * @brief This namespace contains all UDLs for types in the namespace dab.
   */
  namespace literals
    {

    /**
     * @brief Construct a dab::gain representing \p value Decibels
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    gain constexpr operator""_dB(long double value)
      {
      return gain{static_cast<float>(value)};
      }

    /**
     * @brief Construct a dab::gain representing \p value Decibels
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    gain constexpr operator""_dB(unsigned long long value)
      {
      return gain{static_cast<float>(value)};
      }

    }

  }

#endif
