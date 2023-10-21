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

#include "oled.h"
#include "main.h"
#include "spi.h"

u8g2_t screen;

static uint8_t u8x8_stm32_gpio_and_delay(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int,
                                         U8X8_UNUSED void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        HAL_Delay(1);
        break;
    case U8X8_MSG_DELAY_MILLI:
        HAL_Delay(arg_int);
        break;
    case U8X8_MSG_GPIO_DC:
        HAL_GPIO_WritePin(OLED_DATA_CMD_GPIO_Port, OLED_DATA_CMD_Pin, arg_int);
        break;
    case U8X8_MSG_GPIO_RESET:
        HAL_GPIO_WritePin(OLED_RESET_GPIO_Port, OLED_RESET_Pin, arg_int);
        break;
    default:
        break;
    }
    return 1;
}

static uint8_t u8x8_byte_4wire_hw_spi(U8X8_UNUSED u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        HAL_SPI_Transmit(&hspi1, (uint8_t *)arg_ptr, arg_int, 10000);
        break;
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        HAL_GPIO_WritePin(OLED_DATA_CMD_GPIO_Port, OLED_DATA_CMD_Pin, arg_int);
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
    case U8X8_MSG_BYTE_END_TRANSFER:
        // hardware NSS is used, so there is nothing to do here.
        break;
    default:
        return 0;
    }
    return 1;
}

void oled_init()
{
    u8g2_Setup_ssd1306_128x64_noname_f(&screen, U8G2_R0, u8x8_byte_4wire_hw_spi, u8x8_stm32_gpio_and_delay);
    u8g2_InitDisplay(&screen);
    u8g2_SetPowerSave(&screen, 0);
}
