/*
 * Copyright (C) 2017 Opendigitalradio (http://www.opendigitalradio.org/)
 * Copyright (C) 2017 Felix Morgner <felix.morgner@hsr.ch>
 * Copyright (C) 2017 Tobias Stauber <tobias.stauber@hsr.ch>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
   * @see dab::make_device
   */
  using device_ptr = std::unique_ptr<struct device>;

  /**
   * @brief The abstract base for all devices
   *
   * This interface provides the common operations for all devices. It also
   * specifies the semantics of these operations, when applied to a concrete
   * implementation. While it is not guaranteed, that every device supports all
   * operations, all devices guarantee that non-supported operations leave the
   * device in a valid state and return a useful, with regard to the concrete
   * device kind, default value.
   *
   * @since 1.0.0
   */
  struct device
    {

    /**
     * @brief A generic descriptor for devices
     *
     * Device descriptors provide a device type neutral description of available
     * devices. The function dab::descriptors() provides access to a list of
     * descriptors for a selection of types. Device descriptors might be used
     * to implement a device selection mechanism for user facing applications.
     *
     * @since 1.0.3
     */
    struct descriptor
      {
      /**
       * @brief The class relative device ID
       *
       * Most device drivers use this key to differenciate between separate
       * devices if multiple physical devices are available. This field can be
       * used in conjuction with #type, to uniquely identify a specific device.
       *
       * @note The value of this field is not necessarily stable, meaning it is
       * <b>NOT</b> safe to serialize or otherwise store this field across
       * different runs of the consuming application.
       *
       * @since 1.0.3
       */
      std::size_t id;

      /**
       * @brief The serial number of the device
       *
       * Most device drivers provide some for of serial number for their
       * devices. Most often, this serial number is unique for a given
       * kind-vendor combination. RTL-SDR devices are a notable exception, as
       * they often appear to use the same serial number for most devices.
       *
       * @note While most vendors/implementation provide unqiue serial numbers,
       * this is not a hard guarantee. This field should <b>NOT</b> be
       * considered safe safe for serialization.
       *
       * @since 1.0.3
       */
      std::string serial;

      /**
       * @brief The kind of the device (e.g. RTL820)
       *
       * All device implementations provide the 'kind' or 'name' of the product
       * they drive through this field. The value of this field is useful in
       * scenarios, where information has to be presented to the user of a
       * program.
       *
       * @since 1.0.3
       */
      std::string kind;

      /**
       * @brief The manufacturer of the device
       *
       * A device implemetation should provide this field if possible. For
       * software devices, this might be the implementor of the device, while
       * for hardware devices, the manufacturer of the actual device should be
       * provided through this field.
       *
       * @since 1.0.3
       */
      std::string manufacturer;

      /**
       * @brief The C++ type of the device
       *
       * This field provides access to the actual C++ type, that is capable of
       * driving this device. It is useful when trying to filter the list of
       * available devices. It can also be used, when multiple different device
       * types are querried, an the use selects a specific one. Since each
       * device implementation provides its own constructor, with its own
       * arguments, the application has to be able to uniquely decide which
       * driver backen to use.
       *
       * @note The value of this field is dependent on the compiler/platform in
       * use and thus <b>NOT</b> safe for exchange across different application
       * instances.
       *
       * @since 1.0.3
       */
      std::type_index type;

      /**
       * @brief Convert the descriptor into a human readable form
       *
       * This conversion operator provides a human readable representation of
       * a device descriptor, suitable for presentation to users.
       *
       * @since 1.0.3
       */
      explicit operator std::string() const
        {
        return manufacturer + " - " + kind + " - " + std::to_string(id) + " - " + serial;
        }

      /**
       * @brief Compare two device descriptors for equality
       *
       * This operator provide equality comparision for device descriptors.
       * Two descriptors are considered to be equal, if their IDs and types
       * match.
       *
       * @param rhs The descriptor to compare this one to
       *
       * @return @c true iff. rhs is equal to @c *this
       * @since 1.0.3
       */
      bool operator==(descriptor const & rhs) const
        {
        return id == rhs.id && type == rhs.type;
        }

      };

    /**
     * @brief Options that can be enabled/disabled on a device
     *
     * Most devices support ways to enable or disable certain features like
     * automatic gain control, in the case of real hardware, or looping, when
     * working with files. This enumeration defines all known features supported
     * by the current implementation. The functions #enable and #disable can be
     * used to enable or disables these options.
     *
     * @since 1.0.0
     */
    enum struct option : std::uint8_t
      {
      /**
       * @brief Option key for enabling or disabling Automatic Gain Control.
       *
       * Certain hardware devices, like the dab::rtl_device, support automatic
       * adjustment of the receiver gain. This option key can be used to #enable
       * or #disable the automatic gain control on devices that support this
       * feature. If the feature is not supported, nothing will happen.
       *
       * @since 1.0.0
       */
      automatic_gain_control,

      /**
       * @brief Option key to enable looping. Mostly interesting for file
       * devices.
       *
       * File devices, as well as devices that contain/represent a finite amount
       * of sample data, provide a means to repeat their contents indefinitely.
       * This option key can be used to #enable or #disable looping on devices
       * that support this feature. If the feature is not supported, nothing
       * will happen.
       */
      loop
      };

    /**
     * @brief Tune receiver to the specified center frequency.
     *
     * This function is used to tune to a specified center frequency if
     * supported by the device.
     *
     * @note You might want to use one of the frequency literals defined in
     * dab::literals
     *
     * @since  1.0.0
     */
    virtual bool tune(frequency centerFrequency) = 0;

    /**
     * @brief Set the receiver gain to the specified value.
     *
     * If the exact gain is not supported by the device, the closest match will
     * instead be used.
     *
     * @return @c true iff. setting the gain succeeded, @c false otherwise
     *
     * @since  1.0.1
     */
    virtual bool gain(gain gain) = 0;

    /**
     * @brief Get the current receiver gain
     *
     * @return The currently set receiver gain
     *
     * @since  1.0.1
     */
    virtual dab::gain gain() const = 0;

    /**
     * @brief Get the list of gains supported by the device
     *
     * @return A vector containing the gains supported by the device
     *
     * @since  1.0.1
     */
    virtual std::vector<dab::gain> gains() const = 0;

    /**
     * @brief Start sample acquisition
     *
     * This function starts the sample acquisiton process. It blocks until #stop
     * is called.
     *
     * @since  1.0.0
     */
    virtual void run() = 0;

    /**
     * @brief Check if the device is currently running in sample acquisition
     * mode.
     *
     * @since  1.0.1
     */
    virtual bool running() const { return m_running; }

    /**
     * @brief Stop sample acquisition
     *
     * This function stops the sample acquisition process. It never blocks.
     *
     * @since  1.0.0
     */
    virtual void stop() { m_running.store(false); }

    /**
     * @brief Enable a device option
     *
     * @rst
     * This function is used to enable one of the options shown in
     * :ref:`device-option-keys`. Unsupported device options are reported as
     * errors. If an option is already enabled, success is reported.
     * @endrst
     *
     * @par Example
     * @rst
     * .. code-block:: cpp
     *
     *    #include <dab/device/device.h>
     *    // other includes ...
     *
     *    std::unique_ptr make_device() {
     *      return // create a device...
     *    }
     *
     *    int main() {
     *      auto dev = make_device();
     *
     *      if(dev->enable(dab::device::option::loop)) {
     *        std::clog << "Enabled looping!\n";
     *      } else {
     *        std::clog << "Failed to enable looping!\n";
     *      }
     *    }
     * @endrst
     *
     * @returns @c true if enabling the option succeeded, or if the option was
     * already enabled, @c false otherwise.
     *
     * @sa #disable
     * @since  1.0.0
     */
    virtual bool enable(option const & option) = 0;

    /**
     * @brief Disable a device option
     *
     * @rst
     * This function is used to disable one of the options shown in
     * :ref:`device-option-keys`. Unsupported device options are reported as
     * errors. If an option is already disabled, success is reported.
     * @endrst
     *
     * @par Example
     * @rst
     * .. code-block:: cpp
     *
     *    #include <dab/device/device.h>
     *    // other includes ...
     *
     *    std::unique_ptr make_device() {
     *      return // create a device...
     *    }
     *
     *    int main() {
     *      auto dev = make_device();
     *
     *      if(dev->disable(dab::device::option::loop)) {
     *        std::clog << "Disabled looping!\n";
     *      } else {
     *        std::clog << "Failed to disable looping!\n";
     *      }
     *    }
     * @endrst
     *
     * @returns @c true if disabling the option succeeded, or if the option was
     * already disabled, @c false otherwise.
     *
     * @sa #enable
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
      /**
       * @brief Construct a new device
       *
       * This constructor is only to be used by concrete device implementations.
       * It is used to initialiaze the common membmers of all devices, namely
       * the sample output queue (#dab::device::m_samples) an the flag used to
       * interrupt concurrent sample acquisition.
       *
       * @par Example
       * @rst
       * .. code-block:: cpp
       *
       *    #include <dab/device/device.h>
       *
       *    struct custom_device : dab::device {
       *      custom_device(sample_queue_t & samples) : device{samples} {
       *        // custom device initialization ...
       *      }
       *    };
       * @endrst
       *
       * @since  1.0.0
       */
      device(sample_queue_t & samples) : m_samples{samples} { }

      /**
       * @brief The sample output queue
       *
       * This queue is provided to all concrete implementations of #device. In
       * order to publish the baseband samples acquired by a device
       * implementation, the implementation must enqueue these samples into this
       * queue.
       *
       * The queue type is thread-safe, so no additional locking has to be used
       * when enqueueing the samples. Samples are expected to be in I/Q format
       * and their component must be scaled to lie between -1.0f and 1.0f.
       *
       * @par Example
       * @rst
       * .. code-block:: cpp
       *
       *    #include <dab/device/device.h>
       *
       *    struct custom_device : dab::device {
       *      private:
       *        void some_acquisition_callback(std::vector<sample_t> data) {
       *          // ensure scaling ...
       *          // possibly more processing ..
       *          m_samples.enqueue(std::move(data));
       *        }
       *    };
       * @endrst
       *
       * @since  1.0.0
       */
      sample_queue_t & m_samples;

      /**
       * @brief An atomic clag to facilitate stopping of sample acquisition
       *
       * The atomic boolean flag is provided for concrete implementations to
       * facilitate graceful stopping of the sample acquisition process. The
       * default implementation of #stop toggles this flag to false, while the
       * default implementation of #running querries this flag to determine
       * whether the sample acquisition process is currently running.
       *
       * If the sample acquisition process is managed by a different facility,
       * the implementor must also implement #running and #stop accordingly.
       * @par Example
       * @rst
       * .. code-block:: cpp
       *
       *    #include <dab/device/device.h>
       *
       *    struct custom_device : dab::device {
       *      private:
       *        void acquisition_loop() {
       *          while(m_running.load(std::memory_order_acquire)) {
       *            do_acquire_samples();
       *          }
       *        }
       *    };
       * @endrst
       *
       * @since  1.0.0
       */
      std::atomic_bool m_running{};
    };

  /**
   * @brief Get the descriptors for all available devices for the given device
   * types
   *
   * This function retrieves all descriptors of all available devices of all
   * specified types, as if by calling device::descriptors for every provided
   * device type.
   *
   * @par Example
   * @rst
   * .. code-block:: cpp
   *
   *    #include <dab/device/device.h>
   *    #include <dab/device/rtl_file.h>
   *
   *    int main() {
   *      auto && descriptors = dab::descriptors<dab::rtl_file>();
   *
   *      for(auto const & descriptor : descriptors) {
   *        // do something with the descriptor
   *      }
   *    }
   *
   * @endrst
   *
   * @tparam DeviceTypes Zero or more types of SDR devices
   *
   * @note Specifying the same type more than one will result in the same
   * descriptors being present multiple times.
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
   * This function creates a new device on the heap and initializes it with with
   * the given arguments. The device is wrapped into a std::unique_ptr, and can
   * be used accordingly.
   *
   * @par Example
   * @rst
   * .. code-block:: cpp
   *
   *    #include <dab/device/device.h>
   *    #include <dab/device/rtl_file.h>
   *
   *    int main() {
   *      auto && samples = dab::sample_queue_t{};
   *      auto && device = dab::make_device<dab::rtl_file>(
   *        samples,
   *        "someFile.raw"
   *      );
   *
   *      // do something with the device
   *    }
   *
   * @endrst
   *
   * @tparam DeviceType The type of device to create
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

