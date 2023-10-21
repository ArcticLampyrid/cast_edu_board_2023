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

#include "page/main_menu_page.h"
#include "infrared_transmitter.h"
#include "page/about_page.h"
#include "page/buzzer_page.h"
#include "page/infrared_page.h"
#include "page/keyboard_page.h"
#include "page/snake_page.h"
static void on_enter_buzzer()
{
    route_to(&buzzer_page_instance);
}

static void on_enter_keyboard()
{
    route_to(&keyboard_page_instance);
}

static void on_enter_infrared()
{
    route_to(&infrared_page_instance);
}

static void on_enter_snake()
{
    route_to(&snake_page_instance);
}

static void on_enter_about()
{
    route_to(&about_page_instance);
}

menu_item items[] = {
    {
        .text = "Buzzer",
        .on_enter = on_enter_buzzer,
    },
    {
        .text = "Keyboard",
        .on_enter = on_enter_keyboard,
    },
    {
        .text = "Infrared",
        .on_enter = on_enter_infrared,
    },
    {
        .text = "Game: Snake",
        .on_enter = on_enter_snake,
    },
    {
        .text = "About",
        .on_enter = on_enter_about,
    },
};
menu_page main_menu_page_instance("[CAST EDU BOARD 2023]", items, sizeof(items) / sizeof(items[0]));