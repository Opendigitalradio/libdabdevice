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
