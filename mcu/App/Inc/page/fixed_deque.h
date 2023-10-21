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
template <typename TData, size_t capacity> class fixed_deque_t
{
  private:
    TData arr[capacity];
    size_t head;
    size_t tail;

  public:
    fixed_deque_t() noexcept : head(0), tail(0)
    {
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return (head == tail);
    }

    [[nodiscard]] bool full() const noexcept
    {
        return (tail + 1) % capacity == head;
    }

    [[nodiscard]] size_t size() const noexcept
    {
        if (tail >= head)
        {
            return tail - head;
        }
        return capacity - (head - tail);
    }

    bool push_front(const TData &data) noexcept
    {
        if (full())
        {
            return false;
        }
        head = (head + capacity - 1) % capacity;
        arr[head] = data;
        return true;
    }

    bool push_back(const TData &data) noexcept
    {
        if (full())
        {
            return false;
        }
        arr[tail] = data;
        tail = (tail + 1) % capacity;
        return true;
    }

    TData pop_front() noexcept
    {
        if (empty())
        {
            return TData();
        }
        TData output = arr[head];
        head = (head + 1) % capacity;
        return output;
    }

    TData pop_back() noexcept
    {
        if (empty())
        {
            return TData();
        }
        tail = (tail + capacity - 1) % capacity;
        TData output = arr[tail];
        return output;
    }

    const TData &front() const noexcept
    {
        return arr[head];
    }

    const TData &back() const noexcept
    {
        return arr[(tail + capacity - 1) % capacity];
    }
};