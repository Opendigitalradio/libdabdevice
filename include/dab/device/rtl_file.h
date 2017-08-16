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

#ifndef DABDEVICE__RTL_FILE
#define DABDEVICE__RTL_FILE

#include "dab/device/device.h"
#include "dab/types/gain.h"

#include <dab/types/common_types.h>

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

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

    bool gain(dab::gain) override
      {
      return true;
      }

    dab::gain gain() const override
      {
      using namespace dab::literals;
      return 0.0_dB;
      }

    std::vector<dab::gain> gains() const override
      {
      return {};
      }

    void run() override
      {
      m_running.store(true, std::memory_order_release);

      while(m_running)
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

