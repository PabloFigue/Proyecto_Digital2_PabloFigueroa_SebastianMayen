/* 
 * File:   USART_Library.h
 * Author: pablo
 *
 * Created on 25 de julio de 2023, 4:55 p.m.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>

#ifndef USART_LIBRARY_H
#define	USART_LIBRARY_H

#ifdef	__cplusplus
extern "C" {
#endif

void UART_RX_config(uint16_t baudrate);
void UART_TX_config(uint16_t baudrate);
void UART_write_char(char c);
char UART_read_char();
void UART_write_string(const char *str);
char UART_set_value(int byte_value);

#ifdef	__cplusplus
}
#endif

#endif	/* USART_LIBRARY_H */

