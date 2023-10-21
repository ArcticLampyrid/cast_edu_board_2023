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

#include "keyboard.h"
#include "gpio.h"
#include "stm32f4xx_ll_gpio.h"

// helper for buzzer
__WEAK void buzzer_beep();

// helper for scanning keyboard

static void scan_column_in_matrix(GPIO_TypeDef *row_port, uint16_t row_pin, bool *states)
{
    row_port->BSRR = row_pin;

    // When executing too fast, the pin state may not be stable.
    // So we add some NOPs to wait for the pin state to be stable.
    // This is required in Release mode (with optimization enabled).
    __NOP();
    __NOP();
    __NOP();
    __NOP();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"
#pragma ide diagnostic ignored "UnreachableCode"
    if (KB_C1_GPIO_Port == KB_C2_GPIO_Port && KB_C2_GPIO_Port == KB_C3_GPIO_Port && KB_C3_GPIO_Port == KB_C4_GPIO_Port)
    {
        // all columns are in the same port
        // read all pins at once
        auto pins_state = READ_REG(KB_C1_GPIO_Port->IDR);
        states[0] = !!(pins_state & KB_C1_Pin);
        states[1] = !!(pins_state & KB_C2_Pin);
        states[2] = !!(pins_state & KB_C3_Pin);
        states[3] = !!(pins_state & KB_C4_Pin);
    }
    else
    {
        // read pins one by one
        states[0] = HAL_GPIO_ReadPin(KB_C1_GPIO_Port, KB_C1_Pin) == GPIO_PIN_SET;
        states[1] = HAL_GPIO_ReadPin(KB_C2_GPIO_Port, KB_C2_Pin) == GPIO_PIN_SET;
        states[2] = HAL_GPIO_ReadPin(KB_C3_GPIO_Port, KB_C3_Pin) == GPIO_PIN_SET;
        states[3] = HAL_GPIO_ReadPin(KB_C4_GPIO_Port, KB_C4_Pin) == GPIO_PIN_SET;
    }
#pragma clang diagnostic pop

    row_port->BSRR = row_pin << 16u;
}

static void scan_matrix(bool *states)
{
    scan_column_in_matrix(KB_R1_GPIO_Port, KB_R1_Pin, &states[0]);
    scan_column_in_matrix(KB_R2_GPIO_Port, KB_R2_Pin, &states[4]);
    scan_column_in_matrix(KB_R3_GPIO_Port, KB_R3_Pin, &states[8]);
    scan_column_in_matrix(KB_R4_GPIO_Port, KB_R4_Pin, &states[12]);
}

static void get_key_raw_states(bool (&states)[KEY_COUNT])
{
    states[KEY_OK] = HAL_GPIO_ReadPin(KEY_OK_GPIO_Port, KEY_OK_Pin) == GPIO_PIN_RESET;
    states[KEY_USER_1] = HAL_GPIO_ReadPin(KEY_USER_1_GPIO_Port, KEY_USER_1_Pin) == GPIO_PIN_RESET;
    states[KEY_USER_2] = HAL_GPIO_ReadPin(KEY_USER_2_GPIO_Port, KEY_USER_2_Pin) == GPIO_PIN_RESET;
    scan_matrix(&states[KEY_R1C1]);
}

// dispatcher

struct key_state_info_t
{
    key_state state = key_state::released;
    uint32_t at_last_changed = 0;
    uint32_t at_last_pressed = 0;
};

static key_state_info_t key_states[KEY_COUNT]{};

void dispatch_for_keys(const key_handler_group_t (&handlers)[KEY_COUNT])
{
    bool raw_states[KEY_COUNT];
    get_key_raw_states(raw_states);
    for (int key_id = 0; key_id < KEY_COUNT; key_id++)
    {
        key_state_info_t *key = &key_states[key_id];
        auto raw_state = raw_states[key_id];
        auto current_tick = HAL_GetTick();
        if (current_tick - key->at_last_changed < 50)
        {
            continue;
        }
        if (key->state == key_state::released && raw_state)
        {
            // buzzer_beep();
            key->state = key_state::pressed;
            key->at_last_pressed = current_tick;
            key->at_last_changed = current_tick;
            handlers[key_id].on_pressed.trigger(key_state::released);
        }
        else if (key->state != key_state::released && !raw_state)
        {
            auto before = key->state;
            key->state = key_state::released;
            key->at_last_changed = current_tick;
            handlers[key_id].on_released.trigger(before);
        }
        else if (key->state == key_state::pressed && raw_state)
        {
            if (current_tick - key->at_last_pressed >= 2000)
            {
                // buzzer_beep();
                key->state = key_state::long_pressed;
                key->at_last_changed = current_tick;
                handlers[key_id].on_long_pressed.trigger(key_state::pressed);
            }
        }
    }
}

void get_key_states(key_state (&states)[KEY_COUNT])
{
    for (int i = 0; i < KEY_COUNT; i++)
    {
        states[i] = key_states[i].state;
    }
}

key_state get_key_state(key_code_t key)
{
    return key_states[key].state;
}