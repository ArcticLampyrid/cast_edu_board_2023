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

#include "infrared_receiver.h"
#include "scrollable_array.h"
#include "tim.h"

constexpr uint32_t ir_receiver_sampling_freq = 84000000;
constexpr TIM_HandleTypeDef *capture_tim = &htim2;

static scrollable_array<uint32_t, 128> buffer;
constexpr uint32_t nec_repeat_max_interval = 250;

static struct
{
    uint8_t addr;
    uint8_t data;
    uint32_t repeat_count;
    uint32_t at_ticks;
} last_nec_command;

void infrared_receiver_enable()
{
    HAL_TIM_IC_Start_IT(capture_tim, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(capture_tim, TIM_CHANNEL_2);
}

void infrared_receiver_disable()
{
    HAL_TIM_IC_Stop_IT(capture_tim, TIM_CHANNEL_1);
    HAL_TIM_IC_Stop_IT(capture_tim, TIM_CHANNEL_2);
}

static inline bool infrared_receiver_match_period(uint32_t sampling_count, uint16_t period)
{
    uint32_t sampling_period = sampling_count;
    uint32_t sampling_period_min = (period - period / 5) * (ir_receiver_sampling_freq / 1000000);
    uint32_t sampling_period_max = (period + period / 5) * (ir_receiver_sampling_freq / 1000000);
    return sampling_period_min <= sampling_period && sampling_period <= sampling_period_max;
}

static bool infrared_receiver_try_parse_nec(uint32_t end_mark)
{
    if (infrared_receiver_match_period(end_mark, 560))
    {
        if (infrared_receiver_match_period(buffer[buffer.size() - 3], 9000) &&
            infrared_receiver_match_period(buffer[buffer.size() - 2], 2250))
        {
            // NEC repeat
            uint32_t current_ticks = HAL_GetTick();
            uint32_t diff_ticks = current_ticks - last_nec_command.at_ticks;
            if (diff_ticks <= nec_repeat_max_interval)
            {
                last_nec_command.repeat_count++;
                last_nec_command.at_ticks = current_ticks;
                infrared_receiver_on_nec_command(last_nec_command.addr, last_nec_command.data,
                                                 last_nec_command.repeat_count);
            }
            return true;
        }

        auto nec_command_valid = true;
        if (infrared_receiver_match_period(buffer[buffer.size() - 67], 9000) &&
            infrared_receiver_match_period(buffer[buffer.size() - 66], 4500))
        {
            // NEC command
            uint32_t raw_data = 0;
            auto iter = buffer.cend() - 66;
            for (uint8_t i = 0; i < 32; i++)
            {
                iter++;
                if (!infrared_receiver_match_period(*iter, 560))
                {
                    nec_command_valid = false;
                    break;
                }
                iter++;
                if (infrared_receiver_match_period(*iter, 560))
                {
                    // reset
                }
                else if (infrared_receiver_match_period(*iter, 1690))
                {
                    // set
                    raw_data |= 1 << i;
                }
                else
                {
                    nec_command_valid = false;
                    break;
                }
            }
            if (nec_command_valid)
            {
                auto addr = raw_data & 0xFF;
                auto inv_addr = (raw_data >> 8) & 0xFF;
                auto data = (raw_data >> 16) & 0xFF;
                auto inv_data = (raw_data >> 24) & 0xFF;
                if (addr == (~inv_addr & 0xFF) && data == (~inv_data & 0xFF))
                {
                    last_nec_command.addr = addr;
                    last_nec_command.data = data;
                    last_nec_command.repeat_count = 0;
                    last_nec_command.at_ticks = HAL_GetTick();
                    infrared_receiver_on_nec_command(addr, data, 0);
                    return true;
                }
            }
        }
    }
    return false;
}

static void infrared_receiver_try_parse(uint32_t end_mark)
{
    if (infrared_receiver_try_parse_nec(end_mark))
    {
        return;
    }
}

void infrared_receiver_on_captured()
{
    auto active_channel = HAL_TIM_GetActiveChannel(capture_tim);
    switch (active_channel)
    {
    case HAL_TIM_ACTIVE_CHANNEL_1:
        // falling and reset
        buffer[0] =
            __HAL_TIM_GET_COMPARE(capture_tim, TIM_CHANNEL_1) - __HAL_TIM_GET_COMPARE(capture_tim, TIM_CHANNEL_2);
        buffer <<= 1;
        break;
    case HAL_TIM_ACTIVE_CHANNEL_2: {
        // raising
        auto captured = __HAL_TIM_GET_COMPARE(capture_tim, TIM_CHANNEL_2);
        buffer[0] = captured;
        buffer <<= 1;
        infrared_receiver_try_parse(captured);
        break;
    }
    default:
        break;
    }
}