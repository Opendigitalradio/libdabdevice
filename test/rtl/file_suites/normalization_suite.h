#ifndef DABDEVICE_TEST_RTL_FILE__NORMALIZATION_SUITE
#define DABDEVICE_TEST_RTL_FILE__NORMALIZATION_SUITE

#include "constants.h"

#include <dab/device/rtl_file.h>

#include <cute/cute.h>
#include <cute/cute_suite.h>
#include <cutex/descriptive_suite.h>

namespace dab
  {

  namespace test
    {

    namespace rtl
      {

      namespace file
        {

        CUTE_DESCRIPTIVE_STRUCT(normalization_tests)
          {
          static cute::suite suite()
            {
            return {
#define LOCAL_TEST(Test) CUTE_SMEMFUN(descriptive_suite_type, Test)
              LOCAL_TEST(test_samples_are_in_normalization_range),
#undef LOCAL_TEST
            };
            }

          void test_samples_are_in_normalization_range()
            {
            dab::sample_queue_t queue{};
            dab::rtl_file device{queue, kEvenSampleFileName};
            device.run();

            dab::internal::sample_t sample{};
            bool outOfRange{};
            std::size_t size{};

            while(queue.try_dequeue(sample))
              {
              if((sample.imag() > 0.0f && sample.imag() > 1.0f) ||
                 (sample.imag() < 0.0f && sample.imag() < -1.0f) ||
                 (sample.real() > 0.0f && sample.real() > 1.0f) ||
                 (sample.real() < 0.0f && sample.real() < -1.0f))
                {
                outOfRange = true;
                continue;
                }

              ++size;
              }

            ASSERT(!outOfRange);
            ASSERT_EQUAL(4, size);
            }
          };

        }

      }

    }

  }

#endif
