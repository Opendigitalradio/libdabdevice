#ifndef DABDEVICE_TEST_RTL_FILE__LOOPING_SUITE
#define DABDEVICE_TEST_RTL_FILE__LOOPING_SUITE

#include "constants.h"

#include <dab/device/rtl_file.h>

#include <cute/cute.h>
#include <cute/cute_suite.h>
#include <cutex/descriptive_suite.h>

#include <future>

namespace dab
  {

  namespace test
    {

    namespace rtl
      {

      namespace file
        {

        CUTE_DESCRIPTIVE_STRUCT(looping_tests)
          {
          static cute::suite suite()
            {
            return {
#define LOCAL_TEST(Test) CUTE_SMEMFUN(descriptive_suite_type, Test)
              LOCAL_TEST(test_looping),
              LOCAL_TEST(test_non_looping_even_4_samples),
              LOCAL_TEST(test_non_looping_odd_4_samples),
              LOCAL_TEST(test_looping_more_samples),
#undef LOCAL_TEST
            };
            }

          void test_looping()
            {
            dab::rtl_file device{m_queue, kEvenSampleFileName};
            device.enable(dab::device::option::loop);

            auto runner = std::async(std::launch::async, [&]{device.run();});

            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            device.stop();

            runner.get();
            ASSERT(true);
            }

          void test_non_looping_even_4_samples()
            {
            dab::rtl_file device{m_queue, kEvenSampleFileName};
            device.run();

            dab::internal::sample_t ignored{};
            std::size_t count{};

            while(m_queue.try_dequeue(ignored))
              {
              ++count;
              }

            ASSERT_EQUAL(4, count);
            }

          void test_non_looping_odd_4_samples()
            {
            dab::rtl_file device{m_queue, kOddSampleFileName};
            device.run();

            dab::internal::sample_t ignored{};
            std::size_t count{};

            while(m_queue.try_dequeue(ignored))
              {
              ++count;
              }

            ASSERT_EQUAL(4, count);
            }

          void test_looping_more_samples()
            {
            dab::rtl_file device{m_queue, kEvenSampleFileName};
            device.enable(dab::device::option::loop);

            auto runner = std::async(std::launch::async, [&]{device.run();});

            std::this_thread::sleep_for(std::chrono::milliseconds{500});
            device.stop();

            runner.get();

            dab::internal::sample_t ignored{};
            std::size_t count{};

            while(m_queue.try_dequeue(ignored))
              {
              ++count;
              }

            ASSERT_LESS(4, count);
            }

          private:
            dab::sample_queue_t m_queue{};
          };

        }

      }

    }

  }

#endif
