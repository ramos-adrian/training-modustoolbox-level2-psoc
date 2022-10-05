#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"


int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    __enable_irq();
    Cy_SCB_UART_Init(SCB5, &scb_5_config, NULL);
    Cy_SCB_UART_Enable(SCB5);

    for (;;)
    {
        Cy_GPIO_Write(CYBSP_LED4_PORT, CYBSP_LED4_PIN, 0);
        Cy_SCB_UART_PutString(SCB5, "LED ON\n");
        Cy_SysLib_Delay(250);
        Cy_GPIO_Write(CYBSP_LED4_PORT, CYBSP_LED4_PIN, 1);
        Cy_SCB_UART_PutString(SCB5, "LED OFF\n");
        Cy_SysLib_Delay(250);
    }
}
