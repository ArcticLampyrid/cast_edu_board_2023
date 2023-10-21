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

#include "page/snake_page.h"
#include "oled.h"
#include "page/main_menu_page.h"
#include "page/snake_game_over_page.h"
#include "stm32f4xx_hal.h"
snake_page snake_page_instance;
using namespace snake;

snake_page::snake_page()
{
    // this->key_handlers[KEY_OK].on_pressed = [](key_state state) { route_to(&main_menu_page_instance); };
    this->key_handlers[KEY_R3C2].on_pressed = [this](key_state state) {
        if (logic.is_valid_forward(direction_t::up))
        {
            logic.forward(direction_t::up);
            last_move_at = HAL_GetTick();
        }
    };
    this->key_handlers[KEY_R4C2].on_pressed = [this](key_state state) {
        if (logic.is_valid_forward(direction_t::down))
        {
            logic.forward(direction_t::down);
            last_move_at = HAL_GetTick();
        }
    };
    this->key_handlers[KEY_R4C1].on_pressed = [this](key_state state) {
        if (logic.is_valid_forward(direction_t::left))
        {
            logic.forward(direction_t::left);
            last_move_at = HAL_GetTick();
        }
    };
    this->key_handlers[KEY_R4C3].on_pressed = [this](key_state state) {
        if (logic.is_valid_forward(direction_t::right))
        {
            logic.forward(direction_t::right);
            last_move_at = HAL_GetTick();
        }
    };
    this->key_handlers[KEY_OK].on_pressed = [this](key_state state) { logic.abort_game(); };
}

static void draw_cell(uint8_t x, uint8_t y, const cell_state_t &cell)
{
    u8g2_SetDrawColor(&screen, 0);
    u8g2_DrawBox(&screen, x * 8, y * 8, 8, 8);
    u8g2_SetDrawColor(&screen, 1);
    if (cell == cell_state_t::snake)
    {
        u8g2_DrawBox(&screen, x * 8 + 1, y * 8 + 1, 6, 6);
    }
    else if (cell == cell_state_t::food)
    {
        u8g2_DrawCircle(&screen, x * 8 + 4, y * 8 + 4, 3, U8G2_DRAW_ALL);
    }
}

void snake_page::update_ui()
{
    u8g2_SendBuffer(&screen);
}

void snake_page::enter()
{
    logic.reset();

    for (uint8_t x = 0; x < board_width; x++)
    {
        for (uint8_t y = 0; y < board_height; y++)
        {
            auto cell = logic.get_cell(x, y);
            draw_cell(x, y, cell);
        }
    }

    logic.on_cell_updated = [](cell_pos_t pos, const cell_state_t &cell) { draw_cell(pos.x, pos.y, cell); };
    logic.on_game_over = [this](game_over_reason_t reason) {
        snake_game_over_page_instance.reason = reason;
        snake_game_over_page_instance.set_score(logic.get_score());
        route_to(&snake_game_over_page_instance);
    };
}

void snake_page::leave()
{
    logic.on_cell_updated = nullptr;
    logic.on_game_over = nullptr;
}

void snake_page::tick()
{
    auto current = HAL_GetTick();
    if (current - last_move_at > 200)
    {
        last_move_at = current;
        logic.tick();
    }
}
