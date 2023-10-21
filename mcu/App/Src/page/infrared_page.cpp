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

#include "page/infrared_page.h"
#include "assert/gif.h"
#include "infrared_receiver.h"
#include "infrared_transmitter.h"
#include "oled.h"
#include "page/main_menu_page.h"
#include "stm32f4xx_hal.h"
infrared_page infrared_page_instance;

infrared_page::infrared_page()
{
    this->key_handlers[KEY_OK].on_pressed = [](key_state) { route_to(&main_menu_page_instance); };
    this->key_handlers[KEY_R4C1].on_pressed = [this](key_state) {
        this->repeat_at = HAL_GetTick() + 110;
        infrared_transmit_nec_command(0, 0x07);
    };
    this->key_handlers[KEY_R4C1].on_released = [this](key_state) { this->repeat_at = UINT32_MAX; };
    this->key_handlers[KEY_R4C2].on_pressed = [this](key_state) {
        this->repeat_at = UINT32_MAX;
        infrared_transmit_nec_command(0, 0x09);
    };
    this->key_handlers[KEY_R4C3].on_pressed = [this](key_state) {
        this->repeat_at = HAL_GetTick() + 110;
        infrared_transmit_nec_command(0, 0x15);
    };
    this->key_handlers[KEY_R4C3].on_released = [this](key_state) { this->repeat_at = UINT32_MAX; };
}

void infrared_page::update_ui()
{
    if (current_index == index_drawn)
        return;
    u8g2_ClearBuffer(&screen);
    u8g2_SetDrawColor(&screen, 1);
    u8g2_DrawBox(&screen, 0, 0, 128, 64);
    u8g2_SetDrawColor(&screen, 0);
    u8g2_DrawXBM(&screen, 32, 0, gif_width, gif_height, gif[current_index]);
    u8g2_SetDrawColor(&screen, 1);
    u8g2_SendBuffer(&screen);
}

void infrared_page::enter()
{
    current_index = 0;
    index_drawn = size_t(-1);
    infrared_receiver_enable();
}

void infrared_page::leave()
{
    infrared_receiver_disable();
}

void infrared_page::prev_signal()
{
    infrared_page_instance.signal_received_at = HAL_GetTick();
    direction = -1;
}

void infrared_page::next_signal()
{
    infrared_page_instance.signal_received_at = HAL_GetTick();
    direction = 1;
}

void infrared_page::tick()
{
    if (HAL_GetTick() >= repeat_at)
    {
        infrared_transmit_nec_repeat();
        this->repeat_at += 110;
    }
    if (HAL_GetTick() - signal_received_at <= 200)
    {
        if (HAL_GetTick() >= change_frame_at + 80)
        {
            change_frame_at = HAL_GetTick();
            current_index = (current_index + gif_length + direction) % gif_length;
        }
    }
}

void infrared_page::reset_signal()
{
    infrared_page_instance.signal_received_at = 0;
    direction = 1;
    current_index = 0;
}

void infrared_page::on_encoder_changed(int32_t diff)
{
    route_to(&main_menu_page_instance);
}

void infrared_receiver_on_nec_command(uint8_t addr, uint8_t data, [[maybe_unused]] uint32_t repeat_count)
{
    if (addr == 0)
    {
        switch (data)
        {
        case 0x07:
            infrared_page_instance.prev_signal();
            break;
        case 0x15:
            infrared_page_instance.next_signal();
            break;
        case 0x09:
            if (repeat_count == 0)
                infrared_page_instance.reset_signal();
            break;
        default:
            break;
        }
    }
}
