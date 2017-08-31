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

#ifndef DABDEVICE_RTL_DEVICE
#define DABDEVICE_RTL_DEVICE

#include "dab/constants/sample_rate.h"
#include "dab/device/device.h"
#include "dab/types/gain.h"

#include <rtl-sdr.h>

#include <array>
#include <cmath>
#include <future>
#include <stdexcept>
#include <vector>

namespace dab
  {

  namespace internal
    {
    /**
     * @internal
     *
     * @brief RTL-SDR gain control modes
     *
     * @author Felix Morgner
     * @since  1.0.1
     */
    enum struct rtl_gain_control : unsigned char
      {
      automatic = 0, ///< Automatic gain control
      manual = 1, ///< Manual gain control
      };

    /**
     * @internal
     *
     * @brief RTL-SDR AGC modes
     */
    enum struct rtl_agc_mode : unsigned char
      {
      off = 0, ///< AGC deactivated
      on = 1, ///< AGC activated
      };

    /**
     * @internal
     *
     * @brief The callback function for the asynchronous acquisition of samples.
     *
     * @author Felix Morgner
     * @since  1.0.0
     */
    extern "C" void callback(unsigned char * buffer, uint32_t length, void * context);
    }

  /**
   * @author Felix Morgner
   *
   * @brief Concrete implementation of dab::device for RTLSDR USB sticks
   *
   * This header-only implementation enables the user to make use of libdabdecode without
   * requiring librtlsdr when this class is not used.
   */
  struct rtl_device : device
    {
    /**
     * @author Felix Morgner
     *
     * @brief Construct a rtl_device with the target sample queue
     *
     * This constructor initializes the RTLSDR USB stick and couples the device to
     * the specified queue. The caller must guarantee that the queue stays valid for
     * as long as samples are acquired from the device.
     *
     * @param queue The destination queue for the acquired samples
     * @param index The device index
     *
     * @throws std::runtime_exception if either no device can be found, opening the first device fails
     * or the sample rate cannot be set to 2.048 MSps.
     *
     * @since 1.0.0
     */
    explicit rtl_device(sample_queue_t & queue, std::size_t const index = 0) :
      device{queue}
      {
      if(!rtlsdr_get_device_count())
        {
        throw std::runtime_error{"No device found!"};
        }

      if(rtlsdr_open(&m_device, index))
        {
        throw std::runtime_error{"Error opening device!"};
        }

      if(rtlsdr_set_sample_rate(m_device, dab::kDefaultSampleRate))
        {
        throw std::runtime_error{"Error setting sample rate!"};
        }

      auto gaincount = rtlsdr_get_tuner_gains(m_device, nullptr);
      std::vector<int> gains(gaincount);
      rtlsdr_get_tuner_gains(m_device, gains.data());
      m_gains.reserve(gains.size());
      for(auto gain : gains)
        {
        m_gains.push_back(dab::gain(gain / 10.0f));
        }

      if(rtlsdr_set_tuner_gain_mode(m_device, static_cast<int>(internal::rtl_gain_control::manual)))
        {
        throw std::runtime_error{"Error setting gain mode!"};
        }

      if(rtlsdr_set_tuner_gain(m_device, static_cast<int>(m_gains[m_gains.size() / 2].value() * 10)))
        {
        throw std::runtime_error{"Error setting gain!"};
        }

      if(rtlsdr_reset_buffer(m_device))
        {
        throw std::runtime_error{"Error resetting buffers!"};
        }
      }

    ~rtl_device()
      {
      rtlsdr_close(m_device);
      }

    bool tune(frequency centerFrequency) override
      {
      rtlsdr_set_center_freq(m_device, static_cast<std::uint32_t>(centerFrequency));
      return rtlsdr_get_center_freq(m_device) == std::uint32_t(centerFrequency);
      }

    bool gain(dab::gain gain) override
      {
      rtlsdr_set_tuner_gain_mode(m_device, static_cast<int>(internal::rtl_gain_control::manual));
      rtlsdr_set_agc_mode(m_device, static_cast<int>(internal::rtl_agc_mode::off));

      auto const realGain = closest_gain(gain);

      return !rtlsdr_set_tuner_gain(m_device, static_cast<int>(realGain.value() * 10));
      }

    dab::gain gain() const override
      {
      return dab::gain{rtlsdr_get_tuner_gain(m_device) / 10.f};
      }

    std::vector<dab::gain> gains() const override
      {
      return m_gains;
      }

    void run() override
      {
      m_running.store(true, std::memory_order_release);

      auto future = std::async(std::launch::async, [&]{ rtlsdr_read_async(m_device, &internal::callback, this, 0, 0); });

      while(m_running)
        {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

      rtlsdr_cancel_async(m_device);
      }

    bool enable(option const & option) override
      {
      switch(option)
        {
        case option::automatic_gain_control:
          rtlsdr_set_tuner_gain_mode(m_device, static_cast<int>(internal::rtl_gain_control::automatic));
          return !rtlsdr_set_agc_mode(m_device, static_cast<int>(internal::rtl_agc_mode::on));
        default:
          return false;
        }
      }

    bool disable(option const & option) override
      {
      switch(option)
        {
        case option::automatic_gain_control:
          rtlsdr_set_tuner_gain_mode(m_device, static_cast<int>(internal::rtl_gain_control::manual));
          return !rtlsdr_set_agc_mode(m_device, static_cast<int>(internal::rtl_agc_mode::off));
        default:
          return false;
        }
      }

    static std::vector<device::descriptor> descriptors()
      {
      auto serialBuffer = std::array<char, 256>{};
      auto productBuffer = std::array<char, 256>{};
      auto manufacturerBuffer = std::array<char, 256>{};
      auto const nofDevices = rtlsdr_get_device_count();

      auto result = std::vector<device::descriptor>{};
      result.reserve(nofDevices);

      for(auto id = 0ull; id < nofDevices; ++id)
        {
        rtlsdr_get_device_usb_strings(id, manufacturerBuffer.data(), productBuffer.data(),serialBuffer.data());
        result.push_back({id, serialBuffer.data(), productBuffer.data(), manufacturerBuffer.data(), typeid(rtl_device)});
        }

      return result;
      }

    private:
      dab::gain closest_gain(dab::gain target) const
        {
        auto closest = m_gains[0];
        for(auto && current : m_gains)
          {
          auto toClosest = std::abs(target.value() - closest.value());
          auto toCurrent = std::abs(target.value() - current.value());
          if(toCurrent < toClosest)
            {
            closest = current;
            }
          }

        return closest;
        }

      rtlsdr_dev_t * m_device{};
      std::vector<dab::gain> m_gains{};
      std::vector<internal::sample_t> m_sampleBuffer{};

      friend void internal::callback(unsigned char * buffer, std::uint32_t length, void * context);
    };

  namespace internal
    {
    extern "C" void callback(unsigned char * buffer, std::uint32_t length, void * context)
      {
      rtl_device * device = static_cast<rtl_device *>(context);
      auto & samplesQueue = device->m_samples;
      auto & sampleBuffer = device->m_sampleBuffer;

      sampleBuffer.resize(length / 2);

      for(std::uint32_t idx = 0; idx < length; idx += 2)
        {
        sampleBuffer[idx / 2] = internal::sample_t{ float(buffer[idx] - 128) / 128, float(buffer[idx + 1] - 128) / 128 };
        }

      samplesQueue.enqueue(sampleBuffer);
      }
    }

  }

#endif
