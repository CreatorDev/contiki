/*
 * Copyright (c) 2016, Imagination Technologies Limited and/or its
 * affiliated group companies.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* TODO: add documentation */
#include <contiki.h>
#include <clock.h>
#include <pic32.h>
#include <pic32_clock.h>
#include <dev/watchdog.h>
#include <platform-init.h>
#include <debug-uart.h>
#include <pic32_irq.h>
#include <pic32_cn_irq.h>
#include "lpm.h"
#include <dev/ca8210/ca8210-radio.h>
#include "dev/serial-line.h"
#include <net-init.h>
#include <leds.h>
#include <sensors.h>
#include "button-sensor.h"
#include "dev/common-clicks.h"
#include <pic32_i2c.h>
#include <pic32_spi.h>
#include <pic32_uart.h>

#ifndef UART_DEBUG_BAUDRATE
#define UART_DEBUG_BAUDRATE 115200
#endif

#ifdef MOTION_CLICK
SENSORS(&button_sensor, &button_sensor2, &motion_sensor);
#elif PROXIMITY_CLICK
SENSORS(&button_sensor, &button_sensor2, &proximity_sensor);
#else
SENSORS(&button_sensor, &button_sensor2);
#endif

static void
button_callback(void)
{
  if(BUTTON1_CHECK_IRQ()) {
    /* Button1 was pressed */
    button1_isr();
  } else if(BUTTON2_CHECK_IRQ()) {
    /* Button2 was pressed */
    button2_isr();
  }
}

/*---------------------------------------------------------------------------*/
#if defined(MOTION_CLICK) || defined(PROXIMITY_CLICK)
static void
sensor_callback(void)
{
#ifdef MOTION_CLICK
  if(MOTION_SENSOR_CHECK_IRQ()) {
    /* Motion was detected */
    motion_sensor_isr();
  }
#elif PROXIMITY_CLICK
  if(PROXIMITY_SENSOR_CHECK_IRQ()) {
    /* Proximity was detected */
    proximity_sensor_isr();
  }
#endif
}
#endif
/*---------------------------------------------------------------------------*/
#ifdef __USE_LPM__
static void
register_lpm_peripherals(void)
{
  #ifdef __ENABLE_SPI_PORT1_LPM__
  lpm_register_peripheral(&pic32_spi1_periph);
  #endif /* __ENABLE_SPI_PORT1_LPM__ */
  #ifdef __ENABLE_SPI_PORT2_LPM__
  lpm_register_peripheral(&pic32_spi2_periph);
  #endif /* __ENABLE_SPI_PORT2_LPM__ */

  #ifdef __ENABLE_UART_PORT1_LPM__
  lpm_register_peripheral(&pic32_uart1_periph);
  #endif /* __ENABLE_UART_PORT1_LPM__ */
  #ifdef __ENABLE_UART_PORT2_LPM__
  lpm_register_peripheral(&pic32_uart2_periph);
  #endif /* __ENABLE_UART_PORT2_LPM__ */
  #ifdef __ENABLE_UART_PORT3_LPM__
  lpm_register_peripheral(&pic32_uart3_periph);
  #endif /* __ENABLE_UART_PORT3_LPM__ */
  #ifdef __ENABLE_UART_PORT4_LPM__
  lpm_register_peripheral(&pic32_uart4_periph);
  #endif /* __ENABLE_UART_PORT4_LPM__ */

  #ifdef __ENABLE_I2C_PORT1_LPM__
  lpm_register_peripheral(&pic32_i2c1_periph);
  #endif /* __ENABLE_I2C_PORT1_LPM__ */
  #ifdef __ENABLE_I2C_PORT2_LPM__
  lpm_register_peripheral(&pic32_i2c2_periph);
  #endif /* __ENABLE_I2C_PORT2_LPM__ */
}
static void
power_down_peripherals(void)
{
  #ifdef __ENABLE_SPI_PORT1_LPM__
  pic32_spi1_power_down();
  #endif /* __ENABLE_SPI_PORT1_LPM__ */
  #ifdef __ENABLE_SPI_PORT2_LPM__
  pic32_spi2_power_down();
  #endif /* __ENABLE_SPI_PORT2_LPM__ */

  #ifdef __ENABLE_UART_PORT1_LPM__
  pic32_uart1_power_down();
  #endif /* __ENABLE_UART_PORT1_LPM__ */
  #ifdef __ENABLE_UART_PORT2_LPM__
  pic32_uart2_power_down();
  #endif /* __ENABLE_UART_PORT2_LPM__ */
  #ifdef __ENABLE_UART_PORT3_LPM__
  pic32_uart3_power_down();
  #endif /* __ENABLE_UART_PORT3_LPM__ */
  #ifdef __ENABLE_UART_PORT4_LPM__
  pic32_uart4_power_down();
  #endif /* __ENABLE_UART_PORT4_LPM__ */

  #ifdef __ENABLE_I2C_PORT1_LPM__
  pic32_i2c1_power_down();
  #endif /* __ENABLE_I2C_PORT1_LPM__ */
  #ifdef __ENABLE_I2C_PORT2_LPM__
  pic32_i2c2_power_down();
  #endif /* __ENABLE_I2C_PORT2_LPM__ */
}
#endif
/*---------------------------------------------------------------------------*/
int
main(int argc, char **argv)
{
  int32_t r = 0;

  pic32_init();
  watchdog_init();
  clock_init();
  leds_init();
  platform_init();
#ifdef __USE_LPM__
  lpm_init();
  register_lpm_peripherals();
  /*
   * Power down all peripherals that have an API: SPI, I2C, UART.
   * Any call to their init function will power up the peripheral.
   */
  power_down_peripherals();
#endif

  process_init();
  process_start(&etimer_process, NULL);
  ctimer_init();
  rtimer_init();

  pic32_cn_irq_add_callback(button_callback);
#if defined(MOTION_CLICK) || defined(PROXIMITY_CLICK)
  pic32_cn_irq_add_callback(sensor_callback);
#endif

  process_start(&sensors_process, NULL);
  SENSORS_ACTIVATE(button_sensor);
  SENSORS_ACTIVATE(button_sensor2);

  dbg_setup_uart(UART_DEBUG_BAUDRATE);
  net_init();
#ifdef __USE_UART_PORT3_FOR_DEBUG__
  uart3_set_input(serial_line_input_byte);
#elif  __USE_UART_PORT2_FOR_DEBUG__
  uart2_set_input(serial_line_input_byte);
#endif
  serial_line_init();

#ifndef __USE_AVRDUDE__
  watchdog_start();
#endif

  autostart_start(autostart_processes);

  while(1) {
    do {
#ifndef __USE_AVRDUDE__
      watchdog_periodic();
#endif
      r = process_run();
    } while(r > 0);
#ifndef __USE_AVRDUDE__
    watchdog_stop();
    #ifdef __USE_LPM__
    lpm_enter();
    #else
    asm volatile("wait");
    #endif
    watchdog_start();
#endif
  }

  return 0;
}

/*---------------------------------------------------------------------------*/
 ISR(_EXTERNAL_1_VECTOR)
 {
    ca8210_interrupt();
 }
/*---------------------------------------------------------------------------*/
