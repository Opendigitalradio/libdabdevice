#ifndef DABDEVICE__RTL_FILE
#define DABDEVICE__RTL_FILE

#include "dab/device/device.h"

#include <dab/types/common_types.h>

#include <fstream>
#include <stdexcept>
#include <string>

namespace dab
  {

  /**
   * @author Felix Morgner
   *
   * @brief Concrete implementation of dab::device for IQ-sample dumps acquired from RTLSDR USB sticks
   *
   * This class is enables the use of IQ dumps that have been acquired with the rtl_sdr utility that
   * ships as part of librtlsdr.
   */
  struct rtl_file : device
    {
    /**
     * @author Felix Morgner
     *
     * @brief Construct a rtl_file meta device with the target sample queue and filename
     *
     * This constructor initializes the device to the specified queue and open the file with the specified
     * name for reading. The caller must guarantee that the queue stays valid for as long as samples are
     * acquired from the device.
     */
    rtl_file(sample_queue_t & samples, std::string const & filename) :
      device{samples},
      m_filename{filename},
      m_fileStream{filename, std::ios::binary}
      {
      if(!m_fileStream)
        {
        throw std::ios::failure{std::string{"Failed to open file '"} + m_filename + "'."};
        }

      m_fileStream.seekg(0, std::ios::end);
      std::size_t size = m_fileStream.tellg();
      m_fileStream.seekg(0);

      if(size < 2)
        {
        throw std::ios::failure{std::string{"File '"} + m_filename + "' is shorter than 2 bytes."};
        }
      }

    bool tune(frequency) override
      {
      return true;
      }

    void run() override
      {
      m_running.store(true, std::memory_order_release);

      while(m_running.load(std::memory_order_acquire))
        {
        auto sample = get_sample();

        if(m_fileStream.eof())
          {
          if(m_doLoop)
            {
            m_fileStream.seekg(0);
            sample = get_sample();
            }
          else
            {
            stop();
            continue;
            }
          }

        m_samples.enqueue(std::move(sample));
        }
      }

    bool enable(option const & option) override
      {
      if(option == option::loop)
        {
        m_doLoop = true;
        return true;
        }

      return false;
      }

    bool disable(option const & option) override
      {
      if(option == option::loop)
        {
        m_doLoop = false;
        return true;
        }

      return false;
      }

    private:
      internal::sample_t get_sample()
        {
        std::uint8_t real{0};
        std::uint8_t imag{0};

        m_fileStream.read((char *)&real, 1);
        m_fileStream.read((char *)&imag, 1);

        float floating_real = real;
        float floating_imag = imag;

        return internal::sample_t{(floating_real - 128) / 128, (floating_imag - 128) / 128};
        }

      std::string const m_filename;
      std::ifstream m_fileStream;
      bool m_doLoop{};
    };

  }

#endif

