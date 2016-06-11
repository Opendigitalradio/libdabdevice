#ifndef __DAB_DEVICE_DEVICE
#define __DAB_DEVICE_DEVICE

#include "frequency/frequency.h"

#include <types/common_types.h>

#include <atomic>
#include <cstdint>

namespace dab
  {

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
     * @author Felix Morgner
     *
     * @brief This enum defines different options that might be supported by devices.
     */
    enum struct option : std::uint8_t
      {
      automatic_gain_control, ///< Option key for enabling or disabling Automatic Gain Control.
      loop ///< Option key to enable looping. Mostly interesting for file devices.
      };

    /**
     * @author Felix Morgner
     *
     * @brief Tune receiver to the specified center frequency.
     *
     * This function is used to tune to a specified center frequency if supported by the device.
     *
     * @note You might want to use one of the frequency literals defined in dab::literals
     */
    virtual bool tune(frequency centerFrequency) = 0;

    /**
     * @author Felix Morgner
     *
     * @brief Start sample acquisition
     *
     * This function start the sample acquisiton. It blocks until #stop is called.
     */
    virtual void run() = 0;

    /**
     * @author Felix Morgner
     *
     * @brief Stop sample acquisition
     *
     * This function
     */
    virtual void stop() { m_running.store(false, std::memory_order_release); }

    /**
     * @author Felix Morgner
     *
     * @brief Enable the specified option
     *
     * @return true if setting the option succeeded, false otherwise
     */
    virtual bool enable(option const & option) = 0;

    /**
     * @author Felix Morgner
     *
     * @brief Disable the specified option
     *
     * @return true if unsetting the option succeeded, false otherwise
     */
    virtual bool disable(option const & option) = 0;

    protected:
      device(sample_queue_t & samples) : m_samples{samples} { }

      sample_queue_t & m_samples;

      std::atomic_bool m_running{};
    };

  }

#endif

