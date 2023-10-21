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

#include "page/page.h"
page *current_page;
void route_to(page *new_page)
{
    if (current_page != nullptr)
    {
        current_page->leave();
    }
    current_page = new_page;
    current_page->enter();
}

void page::on_encoder_changed(int32_t diff)
{
    // do nothing by default
}

void page::enter()
{
    // do nothing by default
}

void page::leave()
{
    // do nothing by default
}
void page::tick()
{
    // do nothing by default
}
