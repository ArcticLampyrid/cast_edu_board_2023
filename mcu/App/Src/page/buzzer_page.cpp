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

#include "page/buzzer_page.h"
#include "buzzer.h"
#include "oled.h"
#include "page/main_menu_page.h"
buzzer_page buzzer_page_instance;

static tone_info test_tones[] = {
    // 我可以跟在你身后
    {784, 500},
    {880, 500},
    {988, 500},
    {1175, 500},
    {1046, 250},
    {659, 750},
    {784, 500},
    {784, 250},
    // 像影子追着光梦游
    {0, 500},
    {880, 500},
    {578, 500},
    {523, 500},
    {578, 500},
    {523, 250},
    {578, 500},
    {784, 500},
    {659, 250},
    // 我可以等在这路口
    {0, 500},
    {784, 500},
    {880, 500},
    {988, 500},
    {1175, 500},
    {1046, 250},
    {659, 750},
    {784, 500},
    {784, 250},
    // 不管你会不会经过
    {0, 500},
    {880, 500},
    {659, 500},
    {523, 500},
    {578, 500},
    {523, 250},
    {494, 500},
    {523, 500},
    {523, 250},
    // 每当我为你抬起头
    {0, 500},
    {784, 500},
    {880, 500},
    {988, 500},
    {1175, 500},
    {1046, 250},
    {659, 750},
    {784, 500},
    {784, 250},
    // 连眼泪都觉得自由
    {0, 500},
    {880, 500},
    {659, 500},
    {523, 500},
    {578, 500},
    {523, 250},
    {578, 500},
    {784, 500},
    {659, 250},
    // 有的爱像阳光倾落
    {0, 500},
    {784, 500},
    {880, 500},
    {988, 500},
    {1175, 500},
    {1046, 250},
    {659, 500},
    {880, 500},
    {880, 750},
    // 边拥有边失去着
    {659, 250},
    {784, 750},
    {784, 1500},
    {0, 500},
    {698, 500},
    {659, 250},
    {578, 250},
    {523, 500},
};

void buzzer_page::update_ui()
{
    u8g2_ClearBuffer(&screen);
    u8g2_SetFont(&screen, u8g2_font_7x14_tf);
    u8g2_DrawStr(&screen, 26, 14, "Buzzer test");
    u8g2_SetFont(&screen, u8g2_font_6x10_tf);
    u8g2_DrawStr(&screen, 18, 37, "Press OK to play");
    u8g2_DrawStr(&screen, 18, 51, "Rotate to return");
    u8g2_SendBuffer(&screen);
}

void buzzer_page::enter()
{
}

void buzzer_page::leave()
{
    buzzer_start_play(nullptr, 0);
}

buzzer_page::buzzer_page()
{
    this->key_handlers[KEY_OK].on_pressed = [](key_state) {
        buzzer_start_play(test_tones, sizeof(test_tones) / sizeof(tone_info));
    };
}
void buzzer_page::on_encoder_changed(int32_t diff)
{
    route_to(&main_menu_page_instance);
}
