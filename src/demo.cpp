#include "device/rtl_device.h"

#include <fstream>

int main()
  {
  using namespace dab::literals;

  dab::sample_queue_t q{};
  dab::rtl_device d{q};

  d.tune(218640_kHz);
  d.run();

  dab::__internal_common::sample_t s{};

  std::ofstream f{"rtl_device.raw", std::ios::binary | std::ios::trunc};

  while(q.try_dequeue(s))
    {
    auto inphase = s.real();
    auto quadrature = s.imag();

    f.write((char *)&inphase, sizeof(float));
    f.write((char *)&quadrature, sizeof(float));
    }

  f.close();
  }

