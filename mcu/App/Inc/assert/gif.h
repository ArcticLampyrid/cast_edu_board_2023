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
#include <cstddef>
#include <cstdint>
// The gif is download from https://blog.csdn.net/qq_37973192/article/details/125701279
// Contact me if there are any copyright issues.
const constexpr size_t gif_length = 33;
const constexpr uint16_t gif_width = 64;
const constexpr uint16_t gif_height = 64;
extern const unsigned char gif[gif_length][512];