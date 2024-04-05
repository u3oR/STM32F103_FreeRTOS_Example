#include "user_main.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "arm_math.h"
#include "main.h"
#include "string.h"
#include "task.h"
#include "usart.h"
#include <stdbool.h>

void vTask1(void *argument);
void vTask2(void *argument);
void vTask3(void *argument);

const char *txt = "it works!\n";

void user_main(void)
{

    xTaskCreate(vTask1, "vTask1", 128, NULL, 1, NULL);
    xTaskCreate(vTask2, "vTask2", 128, NULL, 1, NULL);
    xTaskCreate(vTask3, "vTask3", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    Error_Handler();
}

__NO_RETURN void vTask1(void *argument)
{
    (void)argument;
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
        vTaskDelay(500);
    }
}

__NO_RETURN void vTask2(void *argument)
{
    (void)argument;
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);
        vTaskDelay(500);
    }
}

__NO_RETURN void vTask3(void *argument)
{
    (void)argument;
    while (1)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)txt, (uint16_t)strlen(txt), 100);
        vTaskDelay(1000);
    }
}
