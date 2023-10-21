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
#include "page/fixed_deque.h"
#include <cstdint>
#include <event_handler.h>
#include <random>
namespace snake
{
constexpr uint8_t board_width = 16;
constexpr uint8_t board_height = 8;
enum class cell_state_t : uint8_t
{
    empty = 0,
    food = 1,
    snake = 2,
};
enum class direction_t : uint8_t
{
    none = 0,
    up = 1,
    right = 2,
    down = 3,
    left = 4,
};
enum class game_over_reason_t : uint8_t
{
    aborted = 0,
    self_eaten = 1,
    wall_hit = 2,
    win = 3,
};
struct cell_pos_t
{
    uint8_t x;
    uint8_t y;
};
class snake_logic_t
{
  private:
    // use 2 bits for each cell
    // optimized for low-memory devices
    uint8_t board[(size_t(board_width) * board_height) >> 2];
    fixed_deque_t<cell_pos_t, size_t(board_width) * board_height> body{};
    cell_pos_t snake_head;
    direction_t snake_direction;

    // do not use mt19937, it's too big for this device
    std::minstd_rand rand_engine;
    uint8_t rand_x();
    uint8_t rand_y();

    void set_cell(uint8_t x, uint8_t y, cell_state_t state) noexcept;
    cell_pos_t generate_new_food() noexcept;

  public:
    snake_logic_t() noexcept;
    [[nodiscard]] cell_state_t get_cell(uint8_t x, uint8_t y) const noexcept;
    void reset() noexcept;
    [[nodiscard]] bool is_valid_forward(direction_t direction) const noexcept;
    void tick() noexcept;
    void forward(direction_t direction) noexcept;
    void set_direction(direction_t direction) noexcept;
    [[nodiscard]] uint16_t get_score() const noexcept;
    void abort_game() const noexcept;
    event_handler_t<game_over_reason_t> on_game_over{};
    event_handler_t<cell_pos_t, cell_state_t> on_cell_updated{};
};
} // namespace snake