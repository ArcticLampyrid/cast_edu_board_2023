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

#include <new>
#include <snake_logic.h>
#include <stm32f4xx.h>
using namespace snake;

uint8_t snake::snake_logic_t::rand_x()
{
    std::uniform_int_distribution<uint8_t> dist(0, board_width - 1);
    return dist(rand_engine);
}

uint8_t snake::snake_logic_t::rand_y()
{
    std::uniform_int_distribution<uint8_t> dist(0, board_height - 1);
    return dist(rand_engine);
}

void snake::snake_logic_t::set_cell(uint8_t x, uint8_t y, cell_state_t state) noexcept
{
    size_t bit_index = (size_t(x) + size_t(y) * board_width);
    size_t index = bit_index >> 2;
    uint8_t bit_offset = (bit_index & 0b11) * 2;
    uint8_t mask = 0b11 << bit_offset;
    uint8_t target = static_cast<uint8_t>(state) << bit_offset;
    this->board[index] = (this->board[index] & ~mask) | target;
}

cell_state_t snake::snake_logic_t::get_cell(uint8_t x, uint8_t y) const noexcept
{
    size_t bit_index = (size_t(x) + size_t(y) * board_width);
    size_t index = bit_index >> 2;
    uint8_t bit_offset = (bit_index & 0b11) * 2;
    uint8_t mask = 0b11 << bit_offset;
    return static_cast<cell_state_t>((this->board[index] & mask) >> bit_offset);
}

cell_pos_t snake::snake_logic_t::generate_new_food() noexcept
{
    uint8_t food_x = rand_x();
    uint8_t food_y = rand_y();
    while (this->get_cell(food_x, food_y) != cell_state_t::empty)
    {
        food_x = rand_x();
        food_y = rand_y();
    }
    this->set_cell(food_x, food_y, cell_state_t::food);
    return {food_x, food_y};
}

snake::snake_logic_t::snake_logic_t() noexcept
    : board(), body(), snake_head(), snake_direction(direction_t::none), rand_engine(SysTick->VAL)
{
    this->snake_head = {rand_x(), rand_y()};
    this->set_cell(snake_head.x, snake_head.y, cell_state_t::snake);
    this->body.push_back(snake_head);
    this->generate_new_food();
}

void snake::snake_logic_t::reset() noexcept
{
    this->~snake_logic_t();
    new (this) snake_logic_t();
}

bool snake::snake_logic_t::is_valid_forward(direction_t direction) const noexcept
{
    cell_pos_t target = snake_head;
    switch (direction)
    {
    case direction_t::up:
        target.y--;
        break;
    case direction_t::right:
        target.x++;
        break;
    case direction_t::down:
        target.y++;
        break;
    case direction_t::left:
        target.x--;
        break;
    case direction_t::none:
        return true;
    }
    if (target.x < 0 || target.x >= board_width || target.y < 0 || target.y >= board_height)
    {
        return false;
    }
    cell_state_t target_state = this->get_cell(target.x, target.y);
    if (target_state == cell_state_t::snake)
    {
        return false;
    }
    return true;
}

void snake::snake_logic_t::tick() noexcept
{
    this->forward(this->snake_direction);
}

void snake::snake_logic_t::forward(direction_t direction) noexcept
{
    this->snake_direction = direction;
    cell_pos_t target = snake_head;
    switch (direction)
    {
    case direction_t::none:
        return;
    case direction_t::up:
        target.y--;
        break;
    case direction_t::right:
        target.x++;
        break;
    case direction_t::down:
        target.y++;
        break;
    case direction_t::left:
        target.x--;
        break;
    }
    if (target.x < 0 || target.x >= board_width || target.y < 0 || target.y >= board_height)
    {
        on_game_over.trigger(game_over_reason_t::wall_hit);
        return;
    }
    cell_state_t target_state = this->get_cell(target.x, target.y);
    if (target_state == cell_state_t::snake)
    {
        on_game_over.trigger(game_over_reason_t::self_eaten);
        return;
    }
    snake_head = target;
    if (target_state != cell_state_t::food)
    {
        cell_pos_t tail = this->body.front();
        this->body.pop_front();
        this->set_cell(tail.x, tail.y, cell_state_t::empty);

        this->body.push_back(target);
        this->set_cell(target.x, target.y, cell_state_t::snake);

        on_cell_updated.trigger(tail, cell_state_t::empty);
        on_cell_updated.trigger(target, cell_state_t::snake);
    }
    else
    {
        this->body.push_back(target);
        this->set_cell(target.x, target.y, cell_state_t::snake);

        if (this->body.size() == size_t(board_width) * board_height)
        {
            on_cell_updated.trigger(target, cell_state_t::snake);
            on_game_over.trigger(game_over_reason_t::win);
            return;
        }

        // generate new food
        cell_pos_t food = generate_new_food();

        on_cell_updated.trigger(target, cell_state_t::snake);
        on_cell_updated.trigger(food, cell_state_t::food);
    }
}

void snake_logic_t::set_direction(direction_t direction) noexcept
{
    this->snake_direction = direction;
}

uint16_t snake_logic_t::get_score() const noexcept
{
    return this->body.size() - 1;
}

void snake_logic_t::abort_game() const noexcept
{
    on_game_over.trigger(game_over_reason_t::aborted);
}
