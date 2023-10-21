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
#include <cstdint>
class animation
{
  private:
    uint32_t start_tick = 0;
    bool done = true;

  protected:
    float current_value;
    float start_value;
    float value_span;
    uint32_t duration_tick = 0;
    virtual float map(float t) = 0;

  public:
    explicit animation(float init_value);
    void update();
    void set(float value);
    void transition(float end_value, uint32_t duration_tick);
    void begin(float start_value, float end_value, uint32_t duration_tick);
    [[nodiscard]] bool is_done() const;
    [[nodiscard]] float value() const;
    [[nodiscard]] float end_value() const;
};
