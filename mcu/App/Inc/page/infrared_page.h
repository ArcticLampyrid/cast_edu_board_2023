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

class infrared_page : public page
{
  private:
    size_t current_index = 0;
    size_t index_drawn = size_t(-1);
    uint32_t repeat_at = UINT32_MAX;
    uint32_t signal_received_at = 0;
    int8_t direction = 0;
    uint32_t change_frame_at = 0;

  public:
    infrared_page();
    void update_ui() override;
    void enter() override;
    void leave() override;
    void prev_signal();
    void next_signal();
    void reset_signal();
    void tick() override;
    void on_encoder_changed(int32_t diff) override;
};
extern infrared_page infrared_page_instance;
