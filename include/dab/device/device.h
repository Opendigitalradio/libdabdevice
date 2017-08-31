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

#ifndef DABDEVICE__DEVICE
#define DABDEVICE__DEVICE

#include "dab/types/frequency.h"
#include "dab/types/gain.h"

#include <dab/types/common_types.h>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <typeindex>
#include <vector>

namespace dab
  {

  /**
   * @brief Convenience alias for std::unique_ptr<device>
   *
   * @see #make_device
   */
  using device_ptr = std::unique_ptr<struct device>;

  /**
   * @author Felix Morgner
   *
   * @brief This is the base class for all device implementations.
   *
   * This is the abstraction for an SDR device. It provides the necessary facilities to acquire baseband
   * samples from different kinds of devices.
   *
   * @note This class defines the interface of all device implementation. It is a pure-virtual class
   * and thus can not be instantiated.
   */
  struct device
    {

    /**
     * @brief A generic descriptor for devices
     *
     * This struct provides access to generic information of a device.
     *
     * @since 1.0.3
     */
    struct descriptor
      {
      /**
       * @brief The class relative device ID
       *
       * @since 1.0.3
       */
      std::size_t id;

      /**
       * @brief The serial number of the device
       *
       * @note May be empty
       *
       * @since 1.0.3
       */
      std::string serial;

      /**
       * @brief The kind of the device (e.g. RTL820)
       *
       * @note Must not be empty
       *
       * @since 1.0.3
       */
      std::string kind;

      /**
       * @brief The manufacturer of the device
       *
       * @note Must not be empty
       *
       * @since 1.0.3
       */
      std::string manufacturer;

      /**
       * @brief The C++ type of the device
       *
       * @since 1.0.3
       */
      std::type_index type;

      /**
       * @brief Convert the descriptor into a human readable form
       *
       * @since 1.0.3
       */
      explicit operator std::string() const
        {
        return manufacturer + " - " + kind + " - " + std::to_string(id) + " - " + serial;
        }
      };

    /**
     * @brief This enum defines different options that might be supported by devices.
     *
     * @author Felix Morgner
     * @since 1.0.0
     */
    enum struct option : std::uint8_t
      {
      automatic_gain_control, ///< Option key for enabling or disabling Automatic Gain Control.
      loop ///< Option key to enable looping. Mostly interesting for file devices.
      };

    /**
     * @brief Tune receiver to the specified center frequency.
     *
     * This function is used to tune to a specified center frequency if supported by the device.
     *
     * @note You might want to use one of the frequency literals defined in dab::literals
     *
     * @author Felix Morgner
     * @since  1.0.0
     */
    virtual bool tune(frequency centerFrequency) = 0;

    /**
     * @brief Set the receiver gain to the specified value.
     *
     * If the exact gain is not supported by the device, the closest match will instead be used.
     *
     * @return @c true iff. setting the gain succeeded, @c false otherwise
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    virtual bool gain(gain gain) = 0;

    /**
     * @brief Get the current receiver gain
     *
     * @return The currently set receiver gain
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    virtual dab::gain gain() const = 0;

    /**
     * @brief Get the list of gains supported by the device
     *
     * @return A vector containing the gains supported by the device
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    virtual std::vector<dab::gain> gains() const = 0;

    /**
     * @brief Start sample acquisition
     *
     * This function starts the sample acquisiton process. It blocks until #stop is called.
     *
     * @author Felix Morgner
     * @since  1.0.0
     */
    virtual void run() = 0;

    /**
     * @brief Check if the device is currently running in sample acquisition mode.
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    virtual bool running() const { return m_running; }

    /**
     * @brief Stop sample acquisition
     *
     * This function stops the sample acquisition process. It never blocks.
     *
     * @author Felix Morgner
     * @since  1.0.0
     */
    virtual void stop() { m_running.store(false); }

    /**
     * @brief Enable the specified option
     *
     * @return @c true if setting the option succeeded, @c false otherwise
     *
     * @author Felix Morgner
     * @since  1.0.0
     */
    virtual bool enable(option const & option) = 0;

    /**
     * @brief Disable the specified option
     *
     * @return @c true if unsetting the option succeeded, @c false otherwise
     *
     * @author Felix Morgner
     * @since  1.0.0
     */
    virtual bool disable(option const & option) = 0;

    /**
     * @brief Get the descriptors for all available devices of the type
     */
    static std::vector<device::descriptor> descriptors()
      {
      return {};
      }

    protected:
      device(sample_queue_t & samples) : m_samples{samples} { }

      sample_queue_t & m_samples;

      std::atomic_bool m_running{};
    };

  /**
   * @brief Get the descriptors for all available devices for the given device types
   *
   * This function retrieves all descriptors of all available devices of all specified types, as if by
   * calling device::descriptors for every provided device type.
   *
   * @tparam DeviceTypes Zero or more types of SDR devices
   * @note Specifying the same type more than one will result in the same descriptors being present
   * multiple times.
   *
   * @since 1.0.3
   */
  template<typename ...DeviceTypes>
  std::vector<device::descriptor> descriptors()
    {
    auto const perTypeDescriptors = std::vector<std::vector<device::descriptor>>{DeviceTypes::descriptors()...};
    auto && result = std::vector<device::descriptor>{};
    for(auto const & descriptorsForType : perTypeDescriptors)
      {
      result.insert(result.end(), descriptorsForType.cbegin(), descriptorsForType.cend());
      }
    return result;
    }

  /**
   * @brief Create a new dynamically allocated device
   *
   * This function creates a new device on the heap and initializes it with with the given arguments.
   *
   * @since 1.0.3
   */
  template<typename DeviceType, typename ...ConstructorArgumentTypes>
  device_ptr make_device(ConstructorArgumentTypes && ...constructorArguments)
    {
    return device_ptr{new DeviceType{std::forward<ConstructorArgumentTypes>(constructorArguments)...}};
    }

  }

#endif

