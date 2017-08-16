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

#include <dab/device/rtl_device.h>

#include <fstream>
#include <future>
#include <iostream>
#include <thread>

void dump(dab::sample_queue_t & queue, std::size_t nofSamples)
  {
  // Allocate a file to dump our samples into
  auto && file = std::ofstream{"rtl_device.raw", std::ios::binary | std::ios::trunc};

  // Allocate a block of memory, so we can dequeue faster
  auto buffer = std::vector<dab::internal::sample_t>(4096);

  (void)nofSamples;

 // Collect samples from the queue
 for(auto i = 0ull; i < nofSamples / 4096; ++i)
   {
   // Dequeue a whole block of samples
   queue.dequeue(buffer);

   // We need to transform the samples back to unsigned chars, since the device normalizes them to floats
   for(auto sample : buffer)
     {
     auto inphase =    static_cast<unsigned char>(sample.real() * 128 + 128);
     auto quadrature = static_cast<unsigned char>(sample.imag() * 128 + 128);

     file.write((char const *)&inphase, sizeof(inphase));
     file.write((char const *)&quadrature, sizeof(quadrature));
     }
   }

 // We are done collecting, close the file
 file.close();
  }

int main() try
  {
  using namespace dab::literals;

  // Create a queue to hold out samples
  auto && queue = dab::sample_queue_t{};

  // Initialialize our RTL-SDR device
  auto && device = dab::rtl_device{queue};

  // List the gains supported by our device
  std::cout << "[libdabdevice] INFO: Supported gains (" << device.gains().size()<< "): ";
  bool first = true;
  for(auto && gain : device.gains())
    {
    if(!first)
      {
      std::cout << ", ";
      }

    std::cout << gain;
    first = false;
    }
  std::cout << '\n';

  // Tune to channel 12C (the SRF DAB+ ensemble in Switzerland)
  device.tune(227360_kHz);

  // Set the gain to around 30 dB, the driver will make sure to find a suiting gain
  device.gain(30_dB);

  // Start acquiring samples from the device
  auto runner = std::async(std::launch::async, [&]{
    std::cout << "[libdabdevice] INFO: Starting sample acquisition with a gain of " << device.gain() << '\n';
    device.run();
  });

  // Start dumping the acquired samples into a file
  dump(queue, 104857600);

  // We have collected 200 MiB of samples, stop the device
  device.stop();
  }
catch(std::runtime_error const & e)
  {
  std::cerr << "[libdabdevice] FATAL: " << e.what() << std::endl;
  }
