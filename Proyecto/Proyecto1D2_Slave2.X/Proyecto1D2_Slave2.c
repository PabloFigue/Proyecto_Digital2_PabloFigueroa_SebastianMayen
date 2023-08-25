/* 
 * File:   Proyecto1D2_Slave2.c
 * Author: Mayen
 *
 * Created on 18 de agosto de 2023, 03:45 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT // Bits de selección del oscilador (Oscilador INTOSCIO: función de E/S en el pin RA6/OSC2/CLKOUT, función de E/S en el pin RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Bit de habilitación del perro guardián (Perro guardián deshabilitado, se puede habilitar mediante el bit SWDTEN del registro WDTCON)
#pragma config PWRTE = OFF      // Bit de habilitación del temporizador de encendido (Temporizador de encendido deshabilitado)
#pragma config MCLRE = OFF      // Bit de selección de la función del pin RE3/MCLR (La función del pin RE3/MCLR es una entrada digital, MCLR está conectado internamente a VDD)
#pragma config CP = OFF         // Bit de protección del código (Protección de código de memoria de programa deshabilitada)
#pragma config CPD = OFF        // Bit de protección de datos (Protección de código de memoria de datos deshabilitada)
#pragma config BOREN = OFF      // Bits de selección del restablecimiento por bajo voltaje (Restablecimiento por bajo voltaje deshabilitado)
#pragma config IESO = OFF       // Bit de selección de conmutación interna/externa (Modo de conmutación interna/externa deshabilitado)
#pragma config FCMEN = OFF      // Bit de habilitación del monitor de reloj a prueba de fallas (Monitor de reloj a prueba de fallas deshabilitado)
#pragma config LVP = OFF        // Bit de habilitación de la programación de voltaje bajo (El pin RB3 tiene E/S digital, se debe utilizar HV en MCLR para la programación)

// CONFIG2
#pragma config BOR4V = BOR40V   // Bit de selección de restablecimiento por bajo voltaje (Restablecimiento por bajo voltaje ajustado a 4.0V)
#pragma config WRT = OFF        // Bits de habilitación de la escritura automática en la memoria de programa Flash (Protección de escritura desactivada)

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "config_ADC.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
uint8_t z;
uint8_t dato;
uint8_t bandera,Puente = 2;
int sensor_Infra = 0;
int sensor_Luz;

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            bandera = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(50);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            if (bandera == 0){
            SSPBUF = sensor_Luz;    
            }else if (bandera == 1){
            SSPBUF = sensor_Infra;    
            }else if (bandera == 2){
                Puente = 0;
            }else if (bandera == 3){
                Puente = 1;
            }
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
   if(ADIF == 1){
        while (!ADIF);
        ADIF = 0;
    }
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    adc_init(0);
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        sensor_Luz = adc_read();
        if (sensor_Luz <= 128){
            //PORTD = 255;
        }else{
            //PORTD = 0;
        }
        if (Puente == 0){
            PORTAbits.RA2 = 1;
            __delay_ms(1000);
            PORTAbits.RA2= 0;
            Puente =2;
        }else if (Puente == 1){
            PORTAbits.RA3 = 1;
            __delay_ms(1000);
            PORTAbits.RA3 = 0;
            Puente = 2;
        }
        
        //__delay_ms(100);
        if (PORTAbits.RA1 == 1){
            __delay_ms(2000);
            if (PORTAbits.RA1 == 1){
                sensor_Infra = 1;
            }else{
                sensor_Infra = 0;
            }
        }
       __delay_ms(100);
       
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0b00001;
    ANSELH = 0;
    
    TRISA = 0b00011;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    
    
    I2C_Slave_Init(0x52);   
    
    OSCCONbits.IRCF = 0B111;
    OSCCONbits.SCS = 1;
   
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
}

