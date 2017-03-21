/*
 * Copyright (c) 2016, Imagination Technologies Limited and/or its affiliated
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
 * \file
 * 	I2C driver for PIC32MX (pic32mx470f512h)
 */

#define __I2C_CODE_TEST__  0

#if __I2C_CODE_TEST__
  #ifdef __32MX470F512H__
    #define __USE_I2C__             1
    #define __USE_I2C_PORT1__       1
    #define __USE_I2C_PORT2__       1
  #endif
#endif /* __I2C_CODE_TEST__ */

#ifdef __USE_I2C__

#include <stdio.h>
#include <stdint.h>
#include <p32xxxx.h>
#include <pic32_clock.h>
#include <pic32_i2c.h>
#include "lpm.h"
/*----------------------------------------------------------------------------------------------*/
#define I2C_PORT_LPM(XX)                                                 \
  static uint32_t pic32_i2c##XX##_state = 0;                             \
  static uint32_t pic32_i2c##XX##_brg = 0;                               \
  int8_t                                                                 \
  pic32_i2c##XX##_power_down(void)                                       \
  {                                                                      \
    /*                                                                   \
     * Save copy of register I2C##XX##CON to restore its state           \
     * while powering up the peripheral.                                 \
     * I2C##XX##BRG is cleared when powering down this peripheral.       \
     */                                                                  \
    pic32_i2c##XX##_state = I2C##XX##CON;                                \
    pic32_i2c##XX##_brg = I2C##XX##BRG;                                  \
                                                                         \
    /* Sleep during idle mode */                                         \
    I2C##XX##CONSET = _I2C##XX##CON_SIDL_MASK;                           \
    I2C##XX##CONCLR = _I2C##XX##CON_ON_MASK;                             \
    PMD5SET = _PMD5_I2C##XX##MD_MASK;                                    \
    return 0;                                                            \
  }                                                                      \
  int8_t                                                                 \
  pic32_i2c##XX##_power_up(void)                                         \
  {                                                                      \
    PMD5CLR = _PMD5_I2C##XX##MD_MASK;                                    \
    I2C##XX##BRG = pic32_i2c##XX##_brg;                                  \
    I2C##XX##CON = pic32_i2c##XX##_state;                                \
    return 0;                                                            \
  }                                                                      \
  lpm_registered_peripheral_t pic32_i2c##XX##_periph = {                 \
    .power_up = pic32_i2c##XX##_power_up,                                \
    .power_down = pic32_i2c##XX##_power_down                             \
  };
/*----------------------------------------------------------------------------------------------*/
#define I2C_PORT_LPM_DUMMY(XX)                                           \
  static int8_t                                                          \
  pic32_i2c##XX##_power_up(void)                                         \
  {                                                                      \
     return 0;                                                           \
  }                                                                      \
  static int8_t                                                          \
  pic32_i2c##XX##_power_down(void)                                       \
  {                                                                      \
     return 0;                                                           \
  }

/*----------------------------------------------------------------------------------------------*/
#define I2C_PORT(XX)                                                     \
  static uint8_t pic32_i2c##XX##_nack_bit = 0;                           \
  uint8_t                                                                \
  pic32_i2c##XX##_bus_idle(void)                                         \
  {                                                                      \
    while(I2C##XX##CONbits.SEN || I2C##XX##CONbits.PEN                   \
                || I2C##XX##CONbits.RSEN                                 \
                || I2C##XX##CONbits.RCEN                                 \
                || I2C##XX##CONbits.ACKEN                                \
                || I2C##XX##STATbits.TRSTAT) {                           \
      ;                                                                  \
    }                                                                    \
  return 0;                                                              \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_init(void)                                             \
  {                                                                      \
    pic32_i2c##XX##_power_up();                                          \
                                                                         \
    IEC1CLR = (_IEC1_I2C##XX##MIE_MASK ) | (_IEC1_I2C##XX##BIE_MASK );   \
    IFS1CLR = (_IFS1_I2C##XX##MIF_MASK ) | (_IFS1_I2C##XX##BIF_MASK );   \
    I2C##XX##CON = 0;                                                    \
    I2C##XX##CONSET = _I2C##XX##CON_SMEN_MASK;                           \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_release(void)                                          \
  {                                                                      \
    I2C##XX##CON = 0;                                                    \
    pic32_i2c##XX##_power_down();                                        \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_set_frequency(uint32_t baudrate)                       \
  {                                                                      \
    I2C##XX##BRG = pic32_clock_calculate_brg(2, baudrate);               \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_send_byte(uint8_t byte)                                \
  {                                                                      \
    I2C##XX##TRN = byte;                                                 \
    pic32_i2c##XX##_bus_idle();                                          \
    while (I2C##XX##STATbits.TBF) {                                      \
      ;                                                                  \
    }                                                                    \
    if(I2C##XX##STATbits.BCL) {                                          \
      printf("Collision ocurred, Data is invalid\n");                    \
      return 1;                                                          \
    }                                                                    \
    if(I2C##XX##STATbits.ACKSTAT) {                                      \
      printf(" NACK recived\n");                                         \
      return 1;                                                          \
    }                                                                    \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_send_bytes(const uint8_t *ptr, uint8_t length)         \
  {                                                                      \
    while(length) {                                                      \
    if(pic32_i2c##XX##_send_byte(*ptr))                                  \
    return 1;                                                            \
    ptr++;                                                               \
    length--;                                                            \
    }                                                                    \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_receive_byte(uint8_t *data)                            \
  {                                                                      \
    I2C##XX##CONSET = _I2C##XX##CON_RCEN_MASK;                           \
    pic32_i2c##XX##_bus_idle();                                          \
    while (!I2C##XX##STATbits.RBF) {                                     \
      ;                                                                  \
    }                                                                    \
    if(I2C##XX##STATbits.BCL) {                                          \
      printf("Collision ocurred, Data is invalid\n");                    \
      return 1;                                                          \
    }                                                                    \
    I2C##XX##CONbits.ACKDT = pic32_i2c##XX##_nack_bit;                   \
    I2C##XX##CONbits.ACKEN = 1;                                          \
    pic32_i2c##XX##_bus_idle();                                          \
    *data = I2C##XX##RCV & 0x000000ff;                                   \
    pic32_i2c##XX##_nack_bit = 0;                                        \
    I2C##XX##CONbits.ACKDT = 0;	                                         \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_set_nack(uint8_t bit)                                  \
  {                                                                      \
    pic32_i2c##XX##_nack_bit = bit & 0x01;                               \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_receive_bytes(uint8_t *ptr, uint8_t length)            \
  {                                                                      \
    uint8_t start_nack_bit = pic32_i2c##XX##_nack_bit;                   \
    pic32_i2c##XX##_nack_bit = 0;                                        \
    while(length) {                                                      \
    if(length == 1 && start_nack_bit)                                    \
        pic32_i2c##XX##_nack_bit = start_nack_bit;                       \
    if(pic32_i2c##XX##_receive_byte(ptr)) {                              \
      return 1;                                                          \
    }                                                                    \
    ptr++;                                                               \
    length--;                                                            \
    }                                                                    \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_send_start(void)                                       \
  {                                                                      \
    I2C##XX##CONSET = _I2C##XX##CON_SEN_MASK;                            \
    while(I2C##XX##CONbits.SEN) {                                        \
      ;                                                                  \
    }                                                                    \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_send_repeated_start(void)                              \
  {                                                                      \
    I2C##XX##CONSET = _I2C##XX##CON_RSEN_MASK;                           \
    while (I2C##XX##CONbits.RSEN) {                                      \
      ;                                                                  \
    }                                                                    \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_send_stop(void)                                        \
  {                                                                      \
    I2C##XX##CONSET = _I2C##XX##CON_PEN_MASK;                            \
    while (I2C##XX##CONbits.PEN) {                                       \
      ;                                                                  \
    }                                                                    \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_master_disable(void)                                   \
  {                                                                      \
    I2C##XX##CONCLR = _I2C##XX##CON_ON_MASK;                             \
    return 0;                                                            \
  }                                                                      \
  uint8_t                                                                \
  pic32_i2c##XX##_master_enable(void)                                    \
  {                                                                      \
    I2C##XX##CONSET = _I2C##XX##CON_ON_MASK;                             \
    return 0;                                                            \
  }
/*--------------------------------------------------------------------------------------------*/

#ifdef __32MX470F512H__
  #ifdef __USE_I2C_PORT1__
    #if defined __USE_LPM__ && defined __ENABLE_I2C_PORT1_LPM__
      I2C_PORT_LPM(1)
    #else
      I2C_PORT_LPM_DUMMY(1)
    #endif /* __USE_LPM__ && __ENABLE_I2C_PORT1_LPM__ */
  I2C_PORT(1)
  #endif /* __USE_I2C_PORT1__ */

  #ifdef __USE_I2C_PORT2__
    #if defined __USE_LPM__ && defined __ENABLE_I2C_PORT2_LPM__
      I2C_PORT_LPM(2)
    #else
      I2C_PORT_LPM_DUMMY(2)
    #endif /* __USE_LPM__ && __ENABLE_I2C_PORT2_LPM__ */
  I2C_PORT(2)
  #endif /* __USE_I2C_PORT2__ */
#endif

#endif /* __USE_I2C__ */
