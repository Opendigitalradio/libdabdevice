#ifndef DABDEVICE_TEST_RTL_FILE__OPTION_SUITE
#define DABDEVICE_TEST_RTL_FILE__OPTION_SUITE

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

        CUTE_DESCRIPTIVE_STRUCT(option_tests)
          {
          static cute::suite suite()
            {
            return {
#define LOCAL_TEST(Test) CUTE_SMEMFUN(descriptive_suite_type, Test)
              LOCAL_TEST(test_enable_invalid_option),
              LOCAL_TEST(test_enable_valid_option),
              LOCAL_TEST(test_disable_invalid_option),
              LOCAL_TEST(test_disable_valid_option),
              LOCAL_TEST(test_looping_is_disabled_by_default),
#undef LOCAL_TEST
            };
            }

          void test_enable_invalid_option()
            {
            rtl_file device{m_queue, kEvenSampleFileName};

            ASSERT(!device.enable(device::option::automatic_gain_control));
            }

          void test_enable_valid_option()
            {
            rtl_file device{m_queue, kEvenSampleFileName};

            ASSERT(device.enable(device::option::loop));
            }

          void test_disable_invalid_option()
            {
            rtl_file device{m_queue, kEvenSampleFileName};

            ASSERT(!device.disable(device::option::automatic_gain_control));
            }

          void test_disable_valid_option()
            {
            rtl_file device{m_queue, kEvenSampleFileName};

            ASSERT(device.disable(device::option::loop));
            }

          void test_looping_is_disabled_by_default()
            {
            rtl_file device{m_queue, kEvenSampleFileName};
            device.run();

            ASSERT(true);
            }

          private:
            sample_queue_t m_queue{};
          };

        }

      }

    }

  }

#endif
