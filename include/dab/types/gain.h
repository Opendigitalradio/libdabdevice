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
  inline std::ostream & operator<<(std::ostream & out, gain const & gain)
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
