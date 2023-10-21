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

#include "event_handler.h"
#include <cstdint>
enum key_code_t
{
    KEY_OK = 0,
    KEY_USER_1,
    KEY_USER_2,
    KEY_R1C1,
    KEY_R1C2,
    KEY_R1C3,
    KEY_R1C4,
    KEY_R2C1,
    KEY_R2C2,
    KEY_R2C3,
    KEY_R2C4,
    KEY_R3C1,
    KEY_R3C2,
    KEY_R3C3,
    KEY_R3C4,
    KEY_R4C1,
    KEY_R4C2,
    KEY_R4C3,
    KEY_R4C4,
    KEY_COUNT,
};

enum class key_state
{
    released,
    pressed,
    long_pressed
};

using key_handler_t = event_handler_t<key_state>;

struct key_handler_group_t
{
    key_handler_t on_pressed{};
    key_handler_t on_released{};
    key_handler_t on_long_pressed{};
};

void dispatch_for_keys(const key_handler_group_t (&handlers)[KEY_COUNT]);
void get_key_states(key_state (&states)[KEY_COUNT]);
key_state get_key_state(key_code_t key);