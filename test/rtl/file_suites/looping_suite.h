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
