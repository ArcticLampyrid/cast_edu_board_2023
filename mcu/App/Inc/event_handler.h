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
#include <functional>
template <typename... Args> class event_handler_t
{
  public:
    using func_type = std::function<void(Args...)>;

    event_handler_t() = default;
    template <typename... ForwardedArgs,
              typename = std::enable_if_t<std::is_constructible_v<func_type, ForwardedArgs...>, int>>
    // NOLINTNEXTLINE(google-explicit-constructor)
    constexpr event_handler_t(ForwardedArgs &&...args) : handler(func_type(std::forward<ForwardedArgs>(args)...))
    {
    }

    template <typename RealFuncType, typename = std::enable_if_t<std::is_constructible_v<func_type, RealFuncType>, int>>
    constexpr event_handler_t &operator=(RealFuncType &&func)
    {
        handler = func;
        return *this;
    }

    template <typename... ForwardedArgs> void trigger(ForwardedArgs &&...args) const
    {
        if (handler)
        {
            // perfect forwarding
            handler(std::forward<ForwardedArgs>(args)...);
        }
    }

  private:
    func_type handler = nullptr;
};
