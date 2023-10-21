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

#include "breathing_light.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include <cstdint>

constexpr TIM_HandleTypeDef *led_controller = &htim3;
constexpr uint32_t channel_red = TIM_CHANNEL_1;
constexpr uint32_t channel_green = TIM_CHANNEL_3;
constexpr uint32_t channel_blue = TIM_CHANNEL_4;

// format: 0x00RRGGBB
static uint32_t color_list[] = {
    0x00500000,
    0x00005000,
    0x000000FF,
    0x004B0082,
};
constexpr uint32_t color_length = sizeof(color_list) / sizeof(color_list[0]);

// for the first half, f(x) = 2^x is used with point x=k*10/64, k=0,1,2,...,63
// for the second half, f(x) = 2^(20-x) is used with point x=k*10/64, k=64,65,...,127
static uint16_t duty[] = {
    1,   1,   1,   1,   2,   2,   2,   2,   2,   3,   3,   3,   4,   4,   5,   5,   6,   6,   7,   8,   9,    10,
    11,  12,  13,  15,  17,  19,  21,  23,  26,  29,  32,  36,  40,  44,  49,  55,  61,  68,  76,  85,  95,   105,
    117, 131, 146, 162, 181, 202, 225, 251, 279, 311, 347, 386, 431, 480, 535, 596, 664, 740, 825, 919, 1024, 919,
    825, 740, 664, 596, 535, 480, 431, 386, 347, 311, 279, 251, 225, 202, 181, 162, 146, 131, 117, 105, 95,   85,
    76,  68,  61,  55,  49,  44,  40,  36,  32,  29,  26,  23,  21,  19,  17,  15,  13,  12,  11,  10,  9,    8,
    7,   6,   6,   5,   5,   4,   4,   3,   3,   3,   2,   2,   2,   2,   2,   1,   1,   1,
};
constexpr uint32_t duty_length = sizeof(duty) / sizeof(duty[0]);

static uint16_t color_index = 0;
static uint16_t duty_index = 0;

void breathing_light_begin()
{
    __HAL_TIM_ENABLE_IT(led_controller, TIM_IT_UPDATE);
    HAL_TIM_PWM_Start(led_controller, channel_red);
    HAL_TIM_PWM_Start(led_controller, channel_green);
    HAL_TIM_PWM_Start(led_controller, channel_blue);
}

void breathing_light_update()
{
    duty_index = (duty_index + 1) % duty_length;
    if (duty_index == 0)
    {
        color_index = (color_index + 1) % color_length;
    }

    auto r = ((color_list[color_index] >> 16) & 0xFF) * duty[duty_index] / 255;
    auto g = ((color_list[color_index] >> 8) & 0xFF) * duty[duty_index] / 255;
    auto b = ((color_list[color_index]) & 0xFF) * duty[duty_index] / 255;

    __HAL_TIM_SET_COMPARE(led_controller, channel_red, r);
    __HAL_TIM_SET_COMPARE(led_controller, channel_green, g);
    __HAL_TIM_SET_COMPARE(led_controller, channel_blue, b);
}