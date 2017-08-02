#ifndef DABDEVICE_TEST_RTL_FILE__CONSTANTS
#define DABDEVICE_TEST_RTL_FILE__CONSTANTS

#include <cstdint>

namespace dab
  {

  namespace test
    {

    namespace rtl
      {

      namespace file
        {

        auto constexpr kEmptyFileName      = "rtl_file_empty";
        auto constexpr kEvenSampleFileName = "rtl_file_even_sample";
        auto constexpr kOddSampleFileName  = "rtl_file_odd_sample";

        std::uint8_t constexpr kEvenSampleData[] = {0, 32, 64, 96, 128, 160, 192, 255};
        std::uint8_t constexpr kOddSampleData[]  = {0, 32, 64, 96, 128, 160, 192, 224, 255};

        }

      }

    }

  }

#endif
