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
#include "ease_out_animation.h"
#include "page.h"
#include <cstddef>

struct menu_item
{
    const char *text = nullptr;
    uint16_t width = 0;
    event_handler_t<> on_enter{};
};

class menu_page : public page
{
  private:
    const char *title = nullptr;
    menu_item *items = nullptr;
    size_t item_count = 0;
    size_t current = 0;
    ease_out_animation top_index{0};
    ease_out_animation shadow_top{0};
    ease_out_animation shadow_bottom{1};
    ease_out_animation shadow_width{0};
    uint16_t font_height;
    uint16_t header_height;
    uint16_t item_height;
    float item_lines;
    void ensure_visible(size_t target_index);

  public:
    menu_page(const char *title, menu_item *items, size_t item_count);
    void focus_with_increment(int increment);
    void focus_to(size_t target_index);
    void update_ui() override;
    void enter() override;
    void leave() override;
    void enter_menu_item();
    void on_encoder_changed(int32_t diff) override;
};