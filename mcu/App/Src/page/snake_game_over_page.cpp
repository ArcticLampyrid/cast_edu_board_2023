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

#include "page/snake_game_over_page.h"
#include "oled.h"
#include "page/main_menu_page.h"
#include "page/snake_page.h"

snake_game_over_page snake_game_over_page_instance;

snake_game_over_page::snake_game_over_page()
{
    this->key_handlers[KEY_OK].on_pressed = [this](key_state) { enter_menu_item(); };
    this->key_handlers[KEY_R4C1].on_pressed = [this](key_state) { focus_with_increment(-1); };
    this->key_handlers[KEY_R4C3].on_pressed = [this](key_state) { focus_with_increment(+1); };
    this->key_handlers[KEY_R4C2].on_pressed = [this](key_state) { enter_menu_item(); };
}

void snake_game_over_page::update_ui()
{
    button_animation.update();

    u8g2_ClearBuffer(&screen);
    u8g2_SetFont(&screen, u8g2_font_7x14_tf);
    switch (reason)
    {
    case snake::game_over_reason_t::aborted:
        u8g2_DrawStr(&screen, 39, 14, "Aborted");
        break;
    case snake::game_over_reason_t::self_eaten:
        u8g2_DrawStr(&screen, 29, 14, "Self eaten");
        break;
    case snake::game_over_reason_t::wall_hit:
        u8g2_DrawStr(&screen, 36, 14, "Wall hit");
        break;
    case snake::game_over_reason_t::win:
        u8g2_DrawStr(&screen, 36, 14, "You win!");
        break;
    }
    u8g2_DrawStr(&screen, score_message_x, 28, score_message);

    u8g2_DrawStr(&screen, 8, 50, "Restart");
    u8g2_DrawStr(&screen, 84, 50, "Exit");

    u8g2_SetDrawColor(&screen, 2);
    u8g2_DrawRBox(&screen, std::lround(2 + 64 * button_animation.value()), 36, 60, 18, 2);
    u8g2_SetDrawColor(&screen, 1);

    u8g2_SendBuffer(&screen);
}

void snake_game_over_page::enter()
{
    button_animation.set(0);
}

void snake_game_over_page::leave()
{
}

void snake_game_over_page::set_score(uint16_t score)
{
    sprintf(score_message, "Score: %d", score);
    u8g2_SetFont(&screen, u8g2_font_7x14_tf);
    score_message_x = (u8g2_GetDisplayWidth(&screen) - u8g2_GetStrWidth(&screen, score_message)) / 2;
}

void snake_game_over_page::focus_with_increment(int increment)
{
    auto value = std::fmod(button_animation.end_value() + (float)increment, 2.0f);
    if (value < 0)
        value += 2;
    button_animation.transition(value, 200);
}

void snake_game_over_page::on_encoder_changed(int32_t diff)
{
    focus_with_increment(diff);
}

void snake_game_over_page::enter_menu_item()
{
    if (button_animation.end_value() < 1)
        route_to(&snake_page_instance);
    else
        route_to(&main_menu_page_instance);
}
