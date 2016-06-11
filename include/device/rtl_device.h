#ifndef __DAB_DEVICE__RTL_DEVICE
#define __DAB_DEVICE__RTL_DEVICE

#include "constants/sample_rate.h"
#include "device/device.h"

#include <rtl-sdr.h>

#include <future>
#include <iostream>
#include <stdexcept>

namespace dab
  {

  namespace _internal_device
    {
    /**
     * @internal
     *
     * @author Felix Morgner
     *
     * @brief The callback function for the asynchronous acquisition of samples.
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
     * @throws std::runtime_exception if either no device can be found, opening the first device fails
     * or the sample rate cannot be set to 2.048 MSps.
     */
    rtl_device(sample_queue_t & queue) :
      device{queue}
      {
      if(!rtlsdr_get_device_count())
        {
        throw std::runtime_error{"No device found!"};
        }

      if(rtlsdr_open(&m_device, 0))
        {
        throw std::runtime_error{"Error opening device!"};
        }

      if(rtlsdr_set_sample_rate(m_device, dab::kDefaultSampleRate))
        {
        throw std::runtime_error{"Error setting sample rate!"};
        }

      rtlsdr_reset_buffer(m_device);
      }

    ~rtl_device()
      {
      rtlsdr_close(m_device);
      }

    bool tune(frequency centerFrequency) override
      {
      rtlsdr_set_tuner_gain_mode(m_device, 0);
      rtlsdr_set_center_freq(m_device, static_cast<std::uint32_t>(centerFrequency));

      return rtlsdr_get_center_freq(m_device);
      }

    void run() override
      {
      m_running.store(true, std::memory_order_release);

      auto future = std::async(std::launch::async, [&]{ rtlsdr_read_async(m_device, &_internal_device::callback, this, 0, 0); });

      auto t = 40000;

      while(m_running.load(std::memory_order_acquire) && t--)
        {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

      rtlsdr_cancel_async(m_device);
      }

    bool enable(option const &) override
      {
      return false;
      }

    bool disable(option const &) override
      {
      return false;
      }

    private:
      rtlsdr_dev_t * m_device{};

      friend void _internal_device::callback(unsigned char * buffer, std::uint32_t length, void * context);
    };

  namespace _internal_device
    {
    extern "C" void callback(unsigned char * buffer, std::uint32_t length, void * context)
      {
      rtl_device * device = static_cast<rtl_device *>(context);
      auto & samplesQueue = device->m_samples;

      for(std::uint32_t idx = 0; idx < length; idx += 2)
        {
        samplesQueue.enqueue(__internal_common::sample_t{
          float(buffer[idx] - 128) / 128,
          float(buffer[idx + 1] - 128) / 128
          });
        }
      }
    }

  }

#endif

