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
#include "main.h"
enum class encoder_direction_t : int8_t
{
    CW = 1,
    CCW = -1
};
class encoder_reader_t
{
    GPIO_TypeDef *pin_a_port;
    uint16_t pin_a;
    GPIO_TypeDef *pin_b_port;
    uint16_t pin_b;
    uint16_t state = 0;
    encoder_direction_t direction = encoder_direction_t::CW;

  public:
    int16_t count = 0;
    encoder_reader_t(GPIO_TypeDef *pinAPort, uint16_t pinA, GPIO_TypeDef *pinBPort, uint16_t pinB);
    void handle_a();
    void handle_b();
};