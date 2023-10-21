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

#include "page/menu_page.h"
#include "oled.h"
#include <cmath>

constexpr u8g2_uint_t screen_width = 128;
constexpr u8g2_uint_t screen_height = 64;

void menu_page::update_ui()
{
    top_index.update();
    shadow_top.update();
    shadow_bottom.update();
    shadow_width.update();

    u8g2_SetFont(&screen, u8g2_font_6x12_tf);
    u8g2_ClearBuffer(&screen);

    // Draw header
    u8g2_SetClipWindow(&screen, 0, 0, screen_width, header_height);
    u8g2_DrawUTF8(&screen, 0, header_height - 1, title);

    // Draw content
    u8g2_SetClipWindow(&screen, 0, header_height, screen_width, screen_height);
    u8g2_uint_t item_bottom = header_height;
    const auto top_index_value = top_index.value();
    auto rounded_offset_y = std::lround((top_index_value - std::floor(top_index_value)) * (float)item_height);
    item_bottom -= static_cast<u8g2_uint_t>(rounded_offset_y);
    for (size_t i = std::floor(top_index_value); i < item_count; i++)
    {
        item_bottom += item_height;
        u8g2_DrawLine(&screen, 2, item_bottom - item_height / 2, 4, item_bottom - item_height / 2);
        u8g2_DrawUTF8(&screen, 10, item_bottom - 1, items[i].text);
    }

    u8g2_SetDrawColor(&screen, 2);
    auto biased_shadow_top = std::lround(shadow_top.value() - top_index_value * (float)item_height);
    auto biased_shadow_bottom = std::lround(shadow_bottom.value() - top_index_value * (float)item_height);
    auto shadow_height = biased_shadow_bottom - biased_shadow_top;
    if (biased_shadow_top < 0)
    {
        shadow_height += biased_shadow_top;
        biased_shadow_top = 0;
    }
    if (shadow_height > 0)
    {
        u8g2_SetDrawColor(&screen, 2);
        u8g2_DrawRBox(&screen, 0, (u8g2_uint_t)biased_shadow_top, (u8g2_uint_t)shadow_width.value(),
                      (u8g2_uint_t)shadow_height, 2);
        u8g2_SetDrawColor(&screen, 1);
    }

    u8g2_SetMaxClipWindow(&screen);
    u8g2_SendBuffer(&screen);
}

void menu_page::enter()
{
}

void menu_page::leave()
{
}

menu_page::menu_page(const char *title, menu_item *items, size_t item_count)
    : title(title), items(items), item_count(item_count)
{
    u8g2_SetFont(&screen, u8g2_font_6x12_tf);
    this->font_height = screen.font_info.max_char_height + screen.font_info.y_offset;
    for (int i = 0; i < item_count; ++i)
    {
        items[i].width = u8g2_GetUTF8Width(&screen, items[i].text);
    }
    this->header_height = font_height;
    this->item_height = font_height;
    this->item_lines = static_cast<float>(screen_height - header_height) / (float)item_height;

    this->key_handlers[KEY_OK].on_pressed = [this](key_state) { this->enter_menu_item(); };
    this->key_handlers[KEY_R3C2].on_pressed = [this](key_state) { this->focus_with_increment(-1); };
    this->key_handlers[KEY_R4C2].on_pressed = [this](key_state) { this->focus_with_increment(1); };
    this->key_handlers[KEY_R4C3].on_pressed = [this](key_state) { this->enter_menu_item(); };

    top_index.set(0);
    shadow_top.set(header_height);
    shadow_bottom.set(float(header_height) + float(item_height));
    shadow_width.set(item_count > 0 ? ((float)items[0].width + 14) : 0);
}

void menu_page::focus_to(size_t target_index)
{
    if (!item_count)
        return;
    if (target_index >= item_count)
    {
        target_index = item_count - 1;
    }
    if (current != target_index)
    {
        auto top_value = float(header_height) + float(target_index) * float(item_height);
        auto bottom_value = top_value + float(item_height);
        if (current < target_index)
        {
            // scroll down
            shadow_top.transition(top_value, 400);
            shadow_bottom.transition(bottom_value, 200);
        }
        else
        {
            // scroll up
            shadow_top.transition(top_value, 200);
            shadow_bottom.transition(bottom_value, 400);
        }
        shadow_width.transition((float)items[target_index].width + 14, 200);
    }
    ensure_visible(target_index);
    current = target_index;
}

void menu_page::focus_with_increment(int increment)
{
    if (increment < 0 && current < (-increment))
        return focus_to(0);
    ;
    if (current + increment >= item_count)
        return focus_to(item_count - 1);
    return focus_to(current + increment);
}

void menu_page::ensure_visible(size_t target_index)
{
    if (!item_count)
        return;
    if (target_index >= item_count)
    {
        target_index = item_count - 1;
    }
    auto i_top = this->top_index.end_value();
    auto i_bottom = i_top + item_lines;
    auto i_target = (float)target_index;
    if (i_top <= i_target && i_target <= i_bottom - 1)
        return;
    if (i_top > i_target)
    {
        this->top_index.transition(i_target, 200);
    }
    else
    {
        this->top_index.transition(i_target - item_lines + 1, 200);
    }
}

void menu_page::on_encoder_changed(int32_t diff)
{
    focus_with_increment(diff);
}

void menu_page::enter_menu_item()
{
    if (current < item_count)
    {
        items[current].on_enter.trigger();
    }
}
