#ifndef __DAB_FREQUENCY__FREQUENCY
#define __DAB_FREQUENCY__FREQUENCY

#include <cstdint>

namespace dab
  {

  /**
   * @author Felix Morgner
   *
   * @brief A type to represent frequencies in Hz
   *
   * This type is a simple wrapper for an unsinged 32-bit integer. It allows to specify
   * that a frequency is required. The exist UDLs to create frequencies in Hz, kHz and MHz
   * in the namespace dab::literals.
   */
  struct frequency
    {
    /**
     * @author Felix Morgner
     *
     * @brief Construct a frequency representing the supplied number of Hertz.
     */
    explicit constexpr frequency(std::uint32_t hertz)
      : m_value{hertz}
      {

      }

    /**
     * @author Felix Morgner
     *
     * @brief Convert a frequency to std::uint32_t for APIs that do not understand dab::frequency
     * like librtlsdr.
     */
    explicit constexpr operator std::uint32_t() const
      {
      return m_value;
      }

    private:
    std::uint32_t m_value;
    };

  /**
   * @namespace literals
   *
   * @author Felix Morgner
   *
   * @brief This namespace contains all UDLs for types in the namespace dab.
   */
  namespace literals
    {

    /**
     * @author Felix Morgner
     *
     * @brief Construct a dab::frequency representing \p value Hertz
     */
    frequency constexpr operator""_Hz(unsigned long long value)
      {
      return frequency{std::uint32_t(value)};
      }

    /**
     * @author Felix Morgner
     *
     * @brief Construct a dab::frequency representing \p value kilo-Hertz
     */
    frequency constexpr operator""_kHz(unsigned long long value)
      {
      return frequency{std::uint32_t(value * 1000)};
      }

    /**
     * @author Felix Morgner
     *
     * @brief Construct a dab::frequency representing \p value kilo-Hertz
     */
    frequency constexpr operator""_kHz(long double value)
      {
      return frequency{std::uint32_t(value * 1000)};
      }

    /**
     * @author Felix Morgner
     *
     * @brief Construct a dab::frequency representing \p value mega-Hertz
     */
    frequency constexpr operator""_MHz(unsigned long long value)
      {
      return frequency{std::uint32_t(value * 1000000)};
      }

    /**
     * @author Felix Morgner
     *
     * @brief Construct a dab::frequency representing \p value mega-Hertz
     */
    frequency constexpr operator""_MHz(long double value)
      {
      return frequency{std::uint32_t(value * 1000000)};
      }

    }

  }

#endif

