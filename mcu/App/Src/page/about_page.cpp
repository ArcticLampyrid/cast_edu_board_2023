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

#include "page/about_page.h"
#include "assert/about_bottom_banner.h"
#include "oled.h"
#include "page/main_menu_page.h"
#include <cmath>
about_page about_page_instance;
about_page::about_page()
{
    for (auto &key_handler : this->key_handlers)
    {
        key_handler.on_pressed = [this](key_state) { route_to(&main_menu_page_instance); };
    }
}

void about_page::update_ui()
{
    auto y = std::lround(top_y.value());
    if (top_y.is_done())
    {
        if (y < 0)
        {
            top_y.transition(0, 250);
        }
        else if (y > 155)
        {
            top_y.transition(155, 250);
        }
    }
    u8g2_ClearBuffer(&screen);
    u8g2_SetFont(&screen, u8g2_font_7x14_tf);
    static const char *lines[] = {"Authors:",        "  Hardware:",       "    @CancerBocchi", "    @Fei Zhao",
                                  "    @Qiuyang Xu", "  Firmware:",       "    @alampy.com",   "    @Shujian Yu",
                                  "Version:",        "  CEB-23-H2.1-F1.0"};
    constexpr auto n_lines = sizeof(lines) / sizeof(lines[0]);
    u8g2_DrawStr(&screen, 46, 14 - y, "About");

    for (int i = 0; i < n_lines; i++)
    {
        u8g2_DrawStr(&screen, 0, 28 + i * 14 - y, lines[i]);
    }

    u8g2_DrawXBM(&screen, 0, 170 - y, about_bottom_banner_width, about_bottom_banner_height, about_bottom_banner);

    u8g2_SendBuffer(&screen);

    top_y.update();
}

void about_page::enter()
{
    top_y.set(0);
}

void about_page::leave()
{
}

void about_page::on_encoder_changed(int32_t diff)
{
    auto end_value = top_y.end_value() + (float)diff * 16;
    top_y.transition(end_value, 520);
}
