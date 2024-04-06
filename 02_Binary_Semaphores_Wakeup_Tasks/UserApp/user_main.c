#include "user_main.h"
#include <stdbool.h>
#include <string.h>

#include "arm_math.h"
#include "main.h"
#include "usart.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "bsp/bsp_uart_debug.h"

SemaphoreHandle_t xBinarySemaphore;

void vTask1(void *argument);
void vTask2(void *argument);
void vTask3(void *argument);

void user_main(void)
{

    xBinarySemaphore = xSemaphoreCreateBinary();

    xTaskCreate(vTask1, "vTask1", 128, NULL, 1, NULL);
    // xTaskCreate(vTask2, "vTask2", 128, NULL, 1, NULL);
    xTaskCreate(vTask3, "vTask3", 128, NULL, 2, NULL);

    vTaskStartScheduler();

    Error_Handler();
}

__NO_RETURN void vTask1(void *argument)
{
    (void)argument;
    while (1)
    {
        user_debug("<TASK1> is Running ...\n");
        
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
        HAL_Delay(1000);
        
        user_debug("<TASK1> wake up Task 3\n");
        
        // 给出二进制信号量，唤醒Task2
        xSemaphoreGive(xBinarySemaphore);
        
        user_debug("<TASK1> completed.\n");
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
        // 等待二进制信号量
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE)
        {
            // Task2被唤醒，执行相关操作...
            user_debug("<TASK3> is Running ...\n");
            // 做一些延时以模拟工作
            HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);
            user_debug("<TASK3> completed.\n");
        }
    }
}
