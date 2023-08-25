/* 
 * Libreria de Configuracion de las interrupciones
 * Author: pablo figueroa
 *
 * Created on 18 de julio de 2023, 
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "INTERRUPT_Library.h"

void ioc_init(char pin){
    
    OPTION_REGbits.nRBPU = 0;   //0 = PORTB pull-ups are enabled by individual PORT latch values //Configuracion PULL-UP 
    
    if (pin == 0){
        TRISBbits.TRISB0 = 1;
        WPUBbits.WPUB0 = 1;
        IOCBbits.IOCB0 = 1;
    }else if (pin == 1){
        TRISBbits.TRISB1 = 1;
        WPUBbits.WPUB1 = 1;
        IOCBbits.IOCB1 = 1;
    }else if (pin == 2){    
        TRISBbits.TRISB2 = 1;
        WPUBbits.WPUB2 = 1;
        IOCBbits.IOCB2 = 1;
    }else if (pin == 3){   
        TRISBbits.TRISB3 = 1;
        WPUBbits.WPUB3 = 1;
        IOCBbits.IOCB3 = 1;
    }else if (pin == 4){
        TRISBbits.TRISB4 = 1;
        WPUBbits.WPUB4 = 1;
        IOCBbits.IOCB4 = 1;
    }else if (pin == 5){
        TRISBbits.TRISB5 = 1;
        WPUBbits.WPUB5 = 1;
        IOCBbits.IOCB5 = 1;
    }else if (pin == 6){
        TRISBbits.TRISB6 = 1;
        WPUBbits.WPUB6 = 1;
        IOCBbits.IOCB6 = 1;
    }else if (pin == 7){
        TRISBbits.TRISB7 = 1;
        WPUBbits.WPUB7 = 1;
        IOCBbits.IOCB7 = 1;
    }
    if (RBIE == 0){
        INTCONbits.RBIF = 0; // Limpiar la bandera de interrupcion del PORTB
        INTCONbits.RBIE = 1; // Habilitar la interrupcion del PORTB on_change
    }
    
}
