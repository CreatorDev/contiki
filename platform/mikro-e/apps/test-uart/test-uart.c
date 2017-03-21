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
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
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
 *  \file
 *  	A test application to print characters on the UART line using pic32 driver
 */

#include <contiki.h>
#include <stdio.h>
#include <pic32_uart.h>

#define BAUD_RATE 115200
#define MESSAGE "UartMessage\n"

/*---------------------------------------------------------------------------*/
PROCESS(test_uart, "Test UART");
AUTOSTART_PROCESSES (&test_uart);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_uart, ev, data)
{
  PROCESS_BEGIN();
  static struct etimer et;
  static char message[] = MESSAGE;
  static int i;
  while(1) {
    etimer_set(&et, CLOCK_SECOND);

    if(pic32_uart3_init(BAUD_RATE, 0) != UART_NO_ERROR) {
      fprintf(stderr, "Uart init failure\n");
      break;
    }

    for(i = 0; i < sizeof(message); i++) {
      if(pic32_uart3_write(message[i]) != UART_NO_ERROR) {
        fprintf(stderr, "Uart write failure\n");
        break;
      }
    }

    if(pic32_uart3_release() != UART_NO_ERROR) {
      fprintf(stderr, "Uart release failure\n");
      break;
    }

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
