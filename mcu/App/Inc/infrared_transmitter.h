// Copyright (C) 2023 ArcticLampyrid <alampy.com>
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once
#include <cstdint>
/**
 * @brief Schedule a pulse to be sent (via infrared_transmitter_on_pulse) after the given delay
 * @param us
 */
extern void infrared_transmitter_schedule_pulse(uint16_t us);
/**
 * @brief Send a pulse to infrared module
 * @see infrared_transmitter_schedule_pulse(uint16_t us)
 */
void infrared_transmitter_on_pulse();
/**
 * @brief Send data via infrared module
 * @param pattern The pattern to send, in us.
 * @param pattern_length The length of the pattern.
 */
void infrared_transmit(uint32_t carrier_freq, uint16_t *pattern, uint32_t pattern_length);
void infrared_transmit_nec_command(uint8_t addr, uint8_t data);
void infrared_transmit_nec_repeat();