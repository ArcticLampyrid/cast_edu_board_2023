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
#include "snake_logic.h"

class snake_game_over_page : public page
{
  private:
    char score_message[12]{};
    uint16_t score_message_x = 0;
    ease_out_animation button_animation{0};
    void focus_with_increment(int increment);

  public:
    snake::game_over_reason_t reason = snake::game_over_reason_t::self_eaten;
    snake_game_over_page();
    void update_ui() override;
    void enter() override;
    void leave() override;
    void set_score(uint16_t score);
    void on_encoder_changed(int32_t diff) override;
    void enter_menu_item();
};

extern snake_game_over_page snake_game_over_page_instance;