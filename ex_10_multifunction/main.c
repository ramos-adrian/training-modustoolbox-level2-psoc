/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Empty PSoC6 Application
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"

cyhal_pwm_t pwm_obj;
int duty_cycle=100;

/* Interrupt callback function */
static void button_isr_user_btn(void *handler_arg, cyhal_gpio_event_t event)
{
	cyhal_gpio_toggle(CYBSP_USER_LED);
}

static void button_isr_ext_btn2(void *handler_arg, cyhal_gpio_event_t event)
{
	duty_cycle-=20;
	if(duty_cycle<0)duty_cycle=100;
	cyhal_pwm_set_duty_cycle(&pwm_obj, duty_cycle, 500);
}

/* GPIO callback initialization structure */
cyhal_gpio_callback_data_t cb_data_user_btn =
{
.callback = button_isr_user_btn,
.callback_arg = NULL
};

cyhal_gpio_callback_data_t cb_data_ext_btn2 =
{
.callback = button_isr_ext_btn2,
.callback_arg = NULL
};


int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    cyhal_gpio_init(P9_2, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLDOWN, 0);
    cyhal_gpio_register_callback(P9_2, &cb_data_user_btn);
    cyhal_gpio_enable_event(P9_2, CYHAL_GPIO_IRQ_FALL, 7, true);
    cyhal_gpio_init(CYBSP_USER_LED, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 0);

    cyhal_gpio_init(P9_6, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLDOWN, 0);
    bool extbtn1_status;
//    int timecounter;
//    int remaining_toggles=0;


    cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLDOWN, 0);
    cyhal_gpio_register_callback(CYBSP_USER_BTN, &cb_data_ext_btn2);
    cyhal_gpio_enable_event(CYBSP_USER_BTN, CYHAL_GPIO_IRQ_FALL, 7, true);

    cyhal_gpio_init(P9_4, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, 1);


    // Initialize PWM on the supplied pin and assign a new clock
    cyhal_pwm_init(&pwm_obj, P9_7, NULL);
    // Start the PWM output
    cyhal_pwm_start(&pwm_obj);
    cyhal_pwm_set_duty_cycle(&pwm_obj, 100, 500);

    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    __enable_irq();

    for (;;)
    {
    	extbtn1_status = cyhal_gpio_read(P9_6);
		if(extbtn1_status){
			for(int f=0;f<10;f++){
			cyhal_gpio_toggle(P9_4);
			cyhal_system_delay_ms(300);
			}
		}
//    	if(extbtn1_status){
//    		cyhal_gpio_write(P9_4, false);
//    		remaining_toggles = 9;
//    		timecounter=0;
//    	}
//    	if(remaining_toggles==0){
//			cyhal_gpio_write(P9_4, true);
//    	}else{
//    		if(timecounter==300000){
//    			cyhal_gpio_toggle(P9_4);
//    			remaining_toggles--;
//    			timecounter=0;
//    		}
//    		timecounter++;
//        	cyhal_system_delay_us(1);
//    	}
    }
}

/* [] END OF FILE */
