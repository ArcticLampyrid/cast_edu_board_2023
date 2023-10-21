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
#include "page.h"
#include "snake_logic.h"
class snake_page : public page
{
  private:
    snake::snake_logic_t logic;
    uint32_t last_move_at{};

  public:
    snake_page();
    void update_ui() override;
    void enter() override;
    void leave() override;
    void tick() override;
};
extern snake_page snake_page_instance;