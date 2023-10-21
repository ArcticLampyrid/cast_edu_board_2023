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
#include <initializer_list>
#include <iterator>

template <typename T, size_t Size> class scrollable_array
{
  private:
    T data[Size];
    size_t head;

  public:
    scrollable_array() : head(0), data()
    {
    }

    // NOLINTNEXTLINE(google-explicit-constructor)
    scrollable_array(const T (&init)[Size]) : head(0)
    {
        for (size_t i = 0; i < Size; i++)
            data[i] = init[i];
    }

    scrollable_array &operator<<=(unsigned int positions)
    {
        if (Size != 0)
            head = (head + positions) % Size;
        return *this;
    }

    scrollable_array &operator>>=(unsigned int positions)
    {
        if (Size != 0)
            head = (head + Size - (positions % Size)) % Size;
        return *this;
    }

    T &operator[](size_t index)
    {
        return data[(head + index) % Size];
    }

    const T &operator[](size_t index) const
    {
        return data[(head + index) % Size];
    }

    [[nodiscard]] constexpr size_t size() const
    {
        return Size;
    }

    class iterator
    {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

        iterator(scrollable_array &arr, size_t idx) : arr(arr), idx(idx)
        {
        }

        reference operator*()
        {
            return arr[idx];
        }
        pointer operator->()
        {
            return &arr[idx];
        }

        iterator &operator++()
        {
            ++idx;
            return *this;
        }
        iterator operator++(int) // NOLINT(cert-dcl21-cpp)
        {
            iterator temp(*this);
            ++*this;
            return temp;
        }

        iterator &operator--()
        {
            --idx;
            return *this;
        }
        iterator operator--(int) // NOLINT(cert-dcl21-cpp)
        {
            iterator temp(*this);
            --*this;
            return temp;
        }

        iterator &operator+=(difference_type n)
        {
            idx += n;
            return *this;
        }
        iterator &operator-=(difference_type n)
        {
            idx -= n;
            return *this;
        }

        iterator operator-(difference_type n) const
        {
            return iterator(arr, idx - n);
        }

        difference_type operator-(const iterator &other) const
        {
            return idx - other.idx;
        }

        reference operator[](difference_type n)
        {
            return arr[idx + n];
        }

        bool operator==(const iterator &other) const
        {
            return idx == other.idx;
        }
        bool operator!=(const iterator &other) const
        {
            return idx != other.idx;
        }
        bool operator<(const iterator &other) const
        {
            return idx < other.idx;
        }
        bool operator>(const iterator &other) const
        {
            return idx > other.idx;
        }
        bool operator<=(const iterator &other) const
        {
            return idx <= other.idx;
        }
        bool operator>=(const iterator &other) const
        {
            return idx >= other.idx;
        }

      private:
        scrollable_array &arr;
        size_t idx;
    };

    iterator begin()
    {
        return iterator(*this, 0);
    }
    iterator end()
    {
        return iterator(*this, Size);
    }

    class const_iterator
    {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type *;
        using reference = const value_type &;

        const_iterator(const scrollable_array &arr, size_t idx) : arr(arr), idx(idx)
        {
        }

        reference operator*() const
        {
            return arr[idx];
        }
        pointer operator->() const
        {
            return &arr[idx];
        }

        const_iterator &operator++()
        {
            ++idx;
            return *this;
        }
        const_iterator operator++(int) // NOLINT(cert-dcl21-cpp)
        {
            const_iterator temp(*this);
            ++*this;
            return temp;
        }

        const_iterator &operator--()
        {
            --idx;
            return *this;
        }
        const_iterator operator--(int) // NOLINT(cert-dcl21-cpp)
        {
            const_iterator temp(*this);
            --*this;
            return temp;
        }

        const_iterator &operator+=(difference_type n)
        {
            idx += n;
            return *this;
        }
        const_iterator &operator-=(difference_type n)
        {
            idx -= n;
            return *this;
        }

        const_iterator operator-(difference_type n) const
        {
            return const_iterator(arr, idx - n);
        }

        difference_type operator-(const const_iterator &other) const
        {
            return idx - other.idx;
        }

        reference operator[](difference_type n) const
        {
            return arr[idx + n];
        }

        bool operator==(const const_iterator &other) const
        {
            return idx == other.idx;
        }
        bool operator!=(const const_iterator &other) const
        {
            return idx != other.idx;
        }
        bool operator<(const const_iterator &other) const
        {
            return idx < other.idx;
        }
        bool operator>(const const_iterator &other) const
        {
            return idx > other.idx;
        }
        bool operator<=(const const_iterator &other) const
        {
            return idx <= other.idx;
        }
        bool operator>=(const const_iterator &other) const
        {
            return idx >= other.idx;
        }

      private:
        const scrollable_array &arr;
        size_t idx;
    };

    const_iterator cbegin() const
    {
        return const_iterator(*this, 0);
    }
    const_iterator cend() const
    {
        return const_iterator(*this, Size);
    }
};

template <typename T, size_t Size>
typename scrollable_array<T, Size>::iterator operator+(typename scrollable_array<T, Size>::iterator it, size_t n)
{
    it += n;
    return it;
}
template <typename T, size_t Size>
typename scrollable_array<T, Size>::iterator operator+(size_t n, typename scrollable_array<T, Size>::iterator it)
{
    it += n;
    return it;
}

template <typename T, size_t Size>
typename scrollable_array<T, Size>::const_iterator operator+(typename scrollable_array<T, Size>::const_iterator it,
                                                             size_t n)
{
    it += n;
    return it;
}
template <typename T, size_t Size>
typename scrollable_array<T, Size>::const_iterator operator+(size_t n,
                                                             typename scrollable_array<T, Size>::const_iterator it)
{
    it += n;
    return it;
}
