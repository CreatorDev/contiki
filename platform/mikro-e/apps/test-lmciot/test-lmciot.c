/*
 * Copyright (c) 2017, Imagination Technologies Limited and/or its affiliated
 * group companies.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 *         A test application to initialise interfaces via LMCIoT
 */

#include "contiki.h"
#include <stdio.h>

#include "dev/leds.h"
#include "letmecreate/letmecreate.h"

/*---------------------------------------------------------------------------*/
PROCESS(test_lmciot, "Test LMCIoT");
AUTOSTART_PROCESSES(&test_lmciot);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_lmciot, ev, data)
{
  PROCESS_BEGIN();


  if(i2c_init() < 0) {
    printf("I2C init failed\n");
    leds_on(LED1);
  }

  if(spi_init() < 0) {
    printf("SPI init failed\n");
    leds_on(LED1);
  }

  if(uart_init() < 0) {
    printf("UART init failed\n");
    leds_on(LED1);
  }

  if(pwm_init(MIKROBUS_1) < 0) {
    printf("PWM init failed\n");
    leds_on(LED1);
  }

  if(gpio_monitor_init() < 0) {
    printf("GPIO monitor init failed\n");
    leds_on(LED1);
  }

  if(led_init() < 0) {
    printf("LED init failed\n");
    leds_on(LED1);
  }

  if(gpio_init(GPIO_AN) < 0) {
    printf("GPIO init failed\n");
    leds_on(LED1);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
