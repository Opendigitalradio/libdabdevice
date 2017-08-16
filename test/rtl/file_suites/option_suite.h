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
