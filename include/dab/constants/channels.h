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

#ifndef DABDEVICE_CONSTANTS_CHANNELS
#define DABDEVICE_CONSTANTS_CHANNELS

#include "dab/types/channel.h"
#include <array>
#include <string>

namespace dab
  {

  /**
   * @brief The list of standard DAB channels
   *
   * @since 1.0.4
   */
  std::array<channel, 38> constexpr kChannels = {{
    { "5A", frequency::kHz(174928ull)},
    { "5B", frequency::kHz(176640ull)},
    { "5C", frequency::kHz(178352ull)},
    { "5D", frequency::kHz(180064ull)},
    { "6A", frequency::kHz(181936ull)},
    { "6B", frequency::kHz(183648ull)},
    { "6C", frequency::kHz(185360ull)},
    { "6D", frequency::kHz(187072ull)},
    { "7A", frequency::kHz(188928ull)},
    { "7B", frequency::kHz(190640ull)},
    { "7C", frequency::kHz(192352ull)},
    { "7D", frequency::kHz(194064ull)},
    { "8A", frequency::kHz(195936ull)},
    { "8B", frequency::kHz(197648ull)},
    { "8C", frequency::kHz(199360ull)},
    { "8D", frequency::kHz(201072ull)},
    { "9A", frequency::kHz(202928ull)},
    { "9B", frequency::kHz(204640ull)},
    { "9C", frequency::kHz(206352ull)},
    { "9D", frequency::kHz(208064ull)},
    {"10A", frequency::kHz(209936ull)},
    {"10B", frequency::kHz(211648ull)},
    {"10C", frequency::kHz(213360ull)},
    {"10D", frequency::kHz(215072ull)},
    {"11A", frequency::kHz(216928ull)},
    {"11B", frequency::kHz(218640ull)},
    {"11C", frequency::kHz(220352ull)},
    {"11D", frequency::kHz(222064ull)},
    {"12A", frequency::kHz(223936ull)},
    {"12B", frequency::kHz(225648ull)},
    {"12C", frequency::kHz(227360ull)},
    {"12D", frequency::kHz(229072ull)},
    {"13A", frequency::kHz(230748ull)},
    {"13B", frequency::kHz(232496ull)},
    {"13C", frequency::kHz(234208ull)},
    {"13D", frequency::kHz(235776ull)},
    {"13E", frequency::kHz(237448ull)},
    {"13F", frequency::kHz(239200ull)},
  }};

  }

#endif
