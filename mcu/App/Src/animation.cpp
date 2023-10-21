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

#include "animation.h"
#include "stm32f4xx_hal.h"
void animation::update()
{
    if (done)
        return;
    auto current_tick = HAL_GetTick();
    float t = (float)(current_tick - start_tick) / (float)duration_tick;
    if (current_tick >= start_tick + duration_tick)
    {
        t = 1;
        done = true;
    }
    current_value = start_value + value_span * map(t);
}

float animation::value() const
{
    return current_value;
}

void animation::begin(float start_value, float end_value, uint32_t duration_tick)
{
    this->current_value = start_value;
    this->start_value = start_value;
    this->value_span = end_value - start_value;
    this->duration_tick = duration_tick;
    this->start_tick = HAL_GetTick();
    done = false;
}

void animation::set(float value)
{
    this->current_value = value;
    this->start_value = value;
    this->value_span = 0;
    this->duration_tick = 0;
    this->start_tick = HAL_GetTick();
    done = true;
}

animation::animation(float init_value) : current_value(init_value), start_value(init_value), value_span(0), done(true)
{
}

void animation::transition(float end_value, uint32_t duration_tick)
{
    this->start_value = current_value;
    this->value_span = end_value - current_value;
    this->duration_tick = duration_tick;
    this->start_tick = HAL_GetTick();
    done = false;
}
float animation::end_value() const
{
    return start_value + value_span;
}
bool animation::is_done() const
{
    return done;
}
