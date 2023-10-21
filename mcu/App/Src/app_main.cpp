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

#include "breathing_light.h"
#include "buzzer.h"
#include "encoder_reader.h"
#include "gpio.h"
#include "infrared_receiver.h"
#include "infrared_transmitter.h"
#include "oled.h"
#include "page/main_menu_page.h"
#include "page/page.h"
#include "spi.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include <app_main.h>

static encoder_reader_t encoder =
    encoder_reader_t(ENCODER_A_GPIO_Port, ENCODER_A_Pin, ENCODER_B_GPIO_Port, ENCODER_B_Pin);

extern "C" void app_pre_init()
{
    // do nothing
}
extern "C" void app_init()
{
    // do nothing
}
extern "C" void app_sys_init()
{
    // do nothing
}

extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
    case ENCODER_A_Pin:
        encoder.handle_a();
        break;
    case ENCODER_B_Pin:
        encoder.handle_b();
        break;
    default:
        break;
    }
}

extern "C" void sys_tick_callback()
{
    // call from SysTick_Handler (interrupt)
    buzzer_tick_1ms();
}

void infrared_transmitter_pulse_init()
{
    TIM4->SR &= ~TIM_SR_UIF; // clear update interrupt flag
    TIM4->DIER |= TIM_IT_UPDATE;
}

void infrared_transmitter_schedule_pulse(uint16_t us)
{
    TIM4->ARR = us - 1;
    TIM4->CNT = 0;
    TIM4->CR1 |= TIM_CR1_CEN;
}

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        // provide a pulse source to the infrared transmitter
        // see also [infrared_transmitter_schedule_pulse]
        infrared_transmitter_on_pulse();
    }
    else if (htim->Instance == TIM3)
    {
        breathing_light_update();
    }
}

extern "C" void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        infrared_receiver_on_captured();
    }
}

extern "C" void app_main()
{
    oled_init();

    // Logo
    u8g2_SetFont(&screen, u8g2_font_10x20_tf);
    u8g2_DrawUTF8(&screen, 19, 40, "CAST 2023");
    u8g2_SendBuffer(&screen);

    infrared_transmitter_pulse_init();
    buzzer_init();
    breathing_light_begin();

    HAL_Delay(500);

    route_to(&main_menu_page_instance);
    for (;;)
    {
        current_page->update_ui();
        int32_t encoder_diff = encoder.count;
        if (encoder_diff != 0)
        {
            encoder.count = 0;
            current_page->on_encoder_changed(-encoder_diff);
        }
        dispatch_for_keys(current_page->key_handlers);
        current_page->tick();
    }
}
