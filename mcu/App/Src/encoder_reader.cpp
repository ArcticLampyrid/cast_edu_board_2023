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

#include <encoder_reader.h>
encoder_reader_t::encoder_reader_t(GPIO_TypeDef *pinAPort, uint16_t pinA, GPIO_TypeDef *pinBPort, uint16_t pinB)
    : pin_a_port(pinAPort), pin_a(pinA), pin_b_port(pinBPort), pin_b(pinB)
{
}
void encoder_reader_t::handle_a()
{
    // Reference
    // https://github.com/nstansby/rpi-rotary-encoder-python/blob/master/encoder.py
    uint8_t new_state = HAL_GPIO_ReadPin(pin_a_port, pin_a) + (HAL_GPIO_ReadPin(pin_b_port, pin_b) << 1);
    switch (state)
    {
    case 0b00:
        if (new_state == 0b01)
        {
            direction = encoder_direction_t::CW;
        }
        else if (new_state == 0b10)
        {
            direction = encoder_direction_t::CCW;
        }
        break;
    case 0b01:
        if (new_state == 0b11)
        {
            direction = encoder_direction_t::CW;
        }
        else if (new_state == 0b00)
        {
            if (direction == encoder_direction_t::CCW)
                count--;
        }
        break;
    case 0b10:
        if (new_state == 0b11)
        {
            direction = encoder_direction_t::CCW;
        }
        else if (new_state == 0b00)
        {
            if (direction == encoder_direction_t::CW)
                count++;
        }
        break;
    case 0b11:
        if (new_state == 0b01)
        {
            direction = encoder_direction_t::CCW;
        }
        else if (new_state == 0b10)
        {
            direction = encoder_direction_t::CW;
        }
        else if (new_state == 0b00)
        {
            if (direction == encoder_direction_t::CW)
                count++;
            else
                count--;
        }
        break;
    default:
        break;
    }
    this->state = new_state;
}
void encoder_reader_t::handle_b()
{
    handle_a();
}