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
#include "damp_animation.h"
#include "page.h"
class about_page : public page
{
  public:
    about_page();
    void update_ui() override;
    void enter() override;
    void leave() override;
    void on_encoder_changed(int32_t diff) override;

  private:
    damp_animation top_y{0};
    uint16_t last_y = UINT16_MAX;
};
extern about_page about_page_instance;