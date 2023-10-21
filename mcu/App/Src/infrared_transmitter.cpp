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

#include "infrared_transmitter.h"
#include "buzzer.h"
#include "tim.h"

enum class infrared_state_t
{
    idle,
    sending,
};

static volatile infrared_state_t state = infrared_state_t::idle;
static volatile uint32_t sending_index = 0;
static volatile uint32_t sending_pattern_length = 0;
static uint16_t *volatile sending_pattern = nullptr;

constexpr uint32_t carrier_generator_base_freq = 84000000;
constexpr TIM_HandleTypeDef *carrier_generator = &htim1;
constexpr uint32_t carrier_generator_channel = TIM_CHANNEL_1;

constexpr uint32_t nec_carrier_freq = 38222;

void infrared_transmitter_on_pulse()
{
    switch (state)
    {
    case infrared_state_t::sending:
        sending_index++;
        if (sending_index >= sending_pattern_length)
        {
            HAL_TIM_PWM_Stop(carrier_generator, carrier_generator_channel);
            state = infrared_state_t::idle;
        }
        else
        {
            infrared_transmitter_schedule_pulse(sending_pattern[sending_index]);
            __HAL_TIM_SET_COMPARE(carrier_generator, carrier_generator_channel, sending_index % 2 == 0 ? 1 : 0);
        }
        break;

    default:
        // Do nothing
        break;
    }
}

void infrared_transmit(uint32_t carrier_freq, uint16_t *pattern, uint32_t pattern_length)
{
    if (pattern == nullptr || pattern_length == 0)
    {
        return;
    }

    // Disable buzzer to avoid interference.
    // For we are using the same timer to generate the carrier wave.
    buzzer_disable();

    __HAL_TIM_SET_PRESCALER(carrier_generator, carrier_generator_base_freq / (4 * carrier_freq) - 1);
    __HAL_TIM_SET_AUTORELOAD(carrier_generator, 4 - 1);
    __HAL_TIM_SET_COUNTER(carrier_generator, 0);
    __HAL_TIM_SET_COMPARE(carrier_generator, carrier_generator_channel, 1); // 25% duty cycle

    sending_index = 0;
    sending_pattern = pattern;
    sending_pattern_length = pattern_length;
    state = infrared_state_t::sending;

    HAL_TIM_PWM_Start(carrier_generator, carrier_generator_channel);
    infrared_transmitter_schedule_pulse(pattern[0]);

    // Wait for the infrared sending to finish.
    while (state != infrared_state_t::idle)
        ;

    buzzer_enable();
}

void infrared_transmit_nec_command(uint8_t addr, uint8_t data)
{
    uint32_t raw_data = (addr) | ((~addr & 0xff) << 8) | (data << 16) | ((~data & 0xff) << 24);
    uint16_t pattern[67];
    pattern[0] = 9000;
    pattern[1] = 4500;
    for (uint8_t i = 0; i < 32; i++)
    {
        pattern[2 + i * 2] = 560;
        if (raw_data & (1 << i))
        {
            pattern[3 + i * 2] = 1690;
        }
        else
        {
            pattern[3 + i * 2] = 560;
        }
    }
    pattern[66] = 560;
    infrared_transmit(nec_carrier_freq, pattern, sizeof(pattern) / sizeof(pattern[0]));
}

void infrared_transmit_nec_repeat()
{
    uint16_t pattern[] = {9000, 2250, 560};
    infrared_transmit(nec_carrier_freq, pattern, sizeof(pattern) / sizeof(pattern[0]));
}