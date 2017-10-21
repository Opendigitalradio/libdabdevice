/*
 * Copyright (C) 2017 Opendigitalradio (http://www.opendigitalradio.org/)
 * Copyright (C) 2017 Felix Morgner <felix.morgner@hsr.ch>
 * Copyright (C) 2017 Tobias Stauber <tobias.stauber@hsr.ch>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DABDEVICE_TYPES_FREQUENCY
#define DABDEVICE_TYPES_FREQUENCY

#include <cstdint>

namespace dab
  {

  /**
   * @brief A type to represent frequencies in Hz
   *
   * This type is a simple wrapper for an unsinged 32-bit integer. It allows to specify
   * that a frequency is required. The exist UDLs to create frequencies in Hz, kHz and MHz
   * in the namespace dab::literals.
   *
   * @since 1.0.0
   */
  struct frequency
    {
    /**
     * @brief Construct a frequency representing the supplied number of Hertz.
     *
     * @since 1.0.0
     */
    explicit constexpr frequency(std::uint32_t hertz)
      : m_value{hertz}
      {

      }

    /**
     * @brief Convert a frequency to std::uint32_t for APIs that do not understand dab::frequency
     * like librtlsdr.
     */
    explicit constexpr operator std::uint32_t() const
      {
      return m_value;
      }

    /**
     * @brief Construct a dab::frequency representing \p value kilo-Hertz
     *
     * @since 1.0.4
     */
    static constexpr frequency kHz(unsigned long long value)
      {
      return frequency{std::uint32_t(value * 1000)};
      }

    /**
     * @brief Construct a dab::frequency representing \p value kilo-Hertz
     *
     * @since 1.0.4
     */
    static constexpr frequency kHz(long double value)
      {
      return frequency{std::uint32_t(value * 1000)};
      }

    /**
     * @brief Construct a dab::frequency representing \p value mega-Hertz
     *
     * @since 1.0.4
     */
    static constexpr frequency MHz(unsigned long long value)
      {
      return frequency{std::uint32_t(value * 1000000)};
      }

    /**
     * @brief Construct a dab::frequency representing \p value mega-Hertz
     *
     * @since 1.0.4
     */
    static constexpr frequency MHz(long double value)
      {
      return frequency{std::uint32_t(value * 1000000)};
      }

    private:
    std::uint32_t m_value;
    };

  /**
   * @namespace literals
   *
   * @brief This namespace contains all UDLs for types in the namespace dab.
   */
  namespace literals
    {

    /**
     * @brief Construct a dab::frequency representing \p value Hertz
     *
     * @since 1.0.0
     */
    frequency constexpr operator""_Hz(unsigned long long value)
      {
      return frequency{std::uint32_t(value)};
      }

    /**
     * @brief Construct a dab::frequency representing \p value kilo-Hertz
     *
     * @since 1.0.0
     */
    frequency constexpr operator""_kHz(unsigned long long value)
      {
      return frequency::kHz(value);
      }

    /**
     * @brief Construct a dab::frequency representing \p value kilo-Hertz
     *
     * @since 1.0.0
     */
    frequency constexpr operator""_kHz(long double value)
      {
      return frequency::kHz(value);
      }

    /**
     * @brief Construct a dab::frequency representing \p value mega-Hertz
     *
     * @since 1.0.0
     */
    frequency constexpr operator""_MHz(unsigned long long value)
      {
      return frequency::MHz(value);
      }

    /**
     * @brief Construct a dab::frequency representing \p value mega-Hertz
     *
     * @since 1.0.0
     */
    frequency constexpr operator""_MHz(long double value)
      {
      return frequency::MHz(value);
      }

    }

  }

#endif
