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

#include "buzzer.h"
#include "tim.h"
#include <cmath>

volatile struct
{
    bool enabled;
    const tone_info *tones;
    uint32_t count;
    uint32_t index;
    uint32_t duration;
} buzzer_state = {
    .enabled = true,
    .tones = nullptr,
    .count = 0,
    .index = static_cast<uint32_t>(-1),
    .duration = 1,
};

static tone_info tones_beep[] = {
    {
        .freq = 2000,
        .duration = 200,
    },
};

void buzzer_init()
{
    buzzer_enable();
}

void buzzer_enable()
{
    __disable_irq();
    buzzer_state.enabled = true;
    __HAL_TIM_SET_AUTORELOAD(&htim1, 100 - 1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    __enable_irq();
}

void buzzer_disable()
{
    __disable_irq();
    buzzer_state.enabled = false;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
    __enable_irq();
}

void buzzer_tick_1ms()
{
    if (!buzzer_state.enabled)
    {
        return;
    }
    if (buzzer_state.tones == nullptr)
    {
        return;
    }
    if (buzzer_state.duration-- != 0)
    {
        return;
    }
    buzzer_state.index++;
    if (buzzer_state.index >= buzzer_state.count)
    {
        buzzer_state.tones = nullptr;
        buzzer_state.count = 0;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
        return;
    }
    auto &tone = buzzer_state.tones[buzzer_state.index];
    buzzer_state.duration = tone.duration;
    if (tone.freq == 0)
    {
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    }
    else
    {
        __HAL_TIM_SET_PRESCALER(&htim1, std::round(840000.f / tone.freq) - 1);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 25); // 25% duty cycle
    }
}

void buzzer_start_play(const tone_info *tones, uint32_t count)
{
    __disable_irq();
    if (!buzzer_state.enabled)
    {
        __enable_irq();
        return;
    }
    buzzer_state.tones = tones;
    buzzer_state.count = count;
    buzzer_state.index = static_cast<uint32_t>(-1);
    buzzer_state.duration = 1;
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
    __enable_irq();
}

void buzzer_beep()
{
    buzzer_start_play(tones_beep, sizeof(tones_beep) / sizeof(tones_beep[0]));
}
