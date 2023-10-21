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

#include "keyboard.h"
#include "u8g2.h"
class page
{
  public:
    virtual ~page() = default;
    virtual void update_ui() = 0;
    virtual void enter();
    virtual void leave();
    virtual void tick();
    virtual void on_encoder_changed(int32_t diff);
    key_handler_group_t key_handlers[KEY_COUNT]{};
};

extern page *current_page;
void route_to(page *new_page);