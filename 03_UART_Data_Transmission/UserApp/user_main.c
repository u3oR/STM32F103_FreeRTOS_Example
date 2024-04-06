#include "user_main.h"
#include <stdbool.h>
#include <string.h>

#include "arm_math.h"
#include "main.h"
#include "usart.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "bsp/bsp_uart_debug.h"

QueueHandle_t xUARTSendQueue;

void vTask1(void *argument);
void vTask2(void *argument);
void vTask3(void *argument);
void vTaskUartSend(void *argument);

void user_main(void)
{
    xUARTSendQueue = xQueueCreate(50, sizeof(char));

    xTaskCreate(vTask1, "vTask1", 128, NULL, 1, NULL);
    xTaskCreate(vTask2, "vTask2", 128, NULL, 1, NULL);
    xTaskCreate(vTask3, "vTask3", 128, NULL, 1, NULL);
    xTaskCreate(vTaskUartSend, "vTaskUartSend", 128, NULL, 2, NULL);

    vTaskStartScheduler();

    Error_Handler();
}

__NO_RETURN void vTask1(void *argument)
{
    (void)argument;
    const char *txt = "vTask1\n";
    while (1)
    {
        taskENTER_CRITICAL();
        for(int i = 0; i < strlen(txt); i++) {
            xQueueSend(xUARTSendQueue, &txt[i], NULL);
        }
        taskEXIT_CRITICAL();
        
        vTaskDelay(500);
    }
}

__NO_RETURN void vTask2(void *argument)
{
    (void)argument;
    const char *txt = "vTask2\n";
    while (1)
    {
        taskENTER_CRITICAL();
        for(int i = 0; i < strlen(txt); i++) {
            xQueueSend(xUARTSendQueue, &txt[i], NULL);
        }
        taskEXIT_CRITICAL();
        
        vTaskDelay(500);
    }
}

__NO_RETURN void vTask3(void *argument)
{
    (void)argument;
    const char *txt = "vTask3\n";
    while (1)
    {
        taskENTER_CRITICAL();
        for(int i = 0; i < strlen(txt); i++) {
            xQueueSend(xUARTSendQueue, &txt[i], NULL);
        }
        taskEXIT_CRITICAL();
        
        vTaskDelay(1000);
    }
}

__NO_RETURN void vTaskUartSend(void *argument)
{
    (void)argument;

    char c;
    
    while (1)
    {
        xQueueReceive(xUARTSendQueue, &c, portMAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 10);
        // vTaskDelay(1000);
    }
}
