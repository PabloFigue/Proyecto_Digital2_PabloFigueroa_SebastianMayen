/* 
 * File:   Proyecto1D2_Slave.c
 * Author: Mayen
 *
 * Created on 15 de agosto de 2023, 05:50 PM
 */
#pragma config FOSC = INTRC_NOCLKOUT // Bits de selecci�n del oscilador (Oscilador INTOSCIO: funci�n de E/S en el pin RA6/OSC2/CLKOUT, funci�n de E/S en el pin RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Bit de habilitaci�n del perro guardi�n (Perro guardi�n deshabilitado, se puede habilitar mediante el bit SWDTEN del registro WDTCON)
#pragma config PWRTE = OFF      // Bit de habilitaci�n del temporizador de encendido (Temporizador de encendido deshabilitado)
#pragma config MCLRE = OFF      // Bit de selecci�n de la funci�n del pin RE3/MCLR (La funci�n del pin RE3/MCLR es una entrada digital, MCLR est� conectado internamente a VDD)
#pragma config CP = OFF         // Bit de protecci�n del c�digo (Protecci�n de c�digo de memoria de programa deshabilitada)
#pragma config CPD = OFF        // Bit de protecci�n de datos (Protecci�n de c�digo de memoria de datos deshabilitada)
#pragma config BOREN = OFF      // Bits de selecci�n del restablecimiento por bajo voltaje (Restablecimiento por bajo voltaje deshabilitado)
#pragma config IESO = OFF       // Bit de selecci�n de conmutaci�n interna/externa (Modo de conmutaci�n interna/externa deshabilitado)
#pragma config FCMEN = OFF      // Bit de habilitaci�n del monitor de reloj a prueba de fallas (Monitor de reloj a prueba de fallas deshabilitado)
#pragma config LVP = OFF        // Bit de habilitaci�n de la programaci�n de voltaje bajo (El pin RB3 tiene E/S digital, se debe utilizar HV en MCLR para la programaci�n)

// CONFIG2
#pragma config BOR4V = BOR40V   // Bit de selecci�n de restablecimiento por bajo voltaje (Restablecimiento por bajo voltaje ajustado a 4.0V)
#pragma config WRT = OFF        // Bits de habilitaci�n de la escritura autom�tica en la memoria de programa Flash (Protecci�n de escritura desactivada)

//*****************************************************************************
// Definici�n e importaci�n de librer�as
//*****************************************************************************
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include <xc.h>
#include "config_ADC.h"
#include "I2C.h"
//*****************************************************************************
// Definici�n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
#define Pin_trig PORTAbits.RA0
#define Pin_echo PORTAbits.RA1
uint16_t distancia,var_distancia;
int time_taken;
uint8_t z;
//******************************************************************************
//Prototipos de funcion
//******************************************************************************
void setup(void);
void Timer1_Init(void);
uint16_t Obtener_Distancia (void);
void setupPWM(void);
char distancia_string[10];
int prueba,bandera;

//*****************************************************************************
// C�digo de Interrupci�n 
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
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupci�n recepci�n/transmisi�n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci�n se complete
            bandera = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepci�n
            __delay_us(50);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = distancia;
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
//******************************************************************************
//Funcion principal
//******************************************************************************
void main() {
    setup();
    Timer1_Init();
    setupPWM();
    CCPR1L = 187;
    
    while(1){      
        distancia = Obtener_Distancia();
        __delay_ms(50);
        
        if (bandera == 1){
            CCPR1L = 125; //125,-90 185,0 250,90
        }else if(bandera == 0) {
            CCPR1L = 187;
        } 
    }
}    
        
void setup(void){
    ANSEL = 0b00000100;
    ANSELbits.ANS2 = 1; // RA2 como entrada anal�gica
    ANSELH = 0;
    
    TRISA = 0b110;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    
    I2C_Slave_Init(0x50);   
    
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 1;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}
void Timer1_Init(void)
{
    T1CON = 0x10; //0b00010000 
}
uint16_t Obtener_Distancia (void)
{
    TMR1H = 0;
    TMR1L = 0;
    
    Pin_trig = 1;
    __delay_us(10);
    Pin_trig = 0;
    
    while(Pin_echo == 0);
    T1CONbits.TMR1ON=1;
    while(Pin_echo == 1);
    T1CONbits.TMR1ON=0;
    
    time_taken= (TMR1L | (TMR1H<<8)); /*read the content of Timer1 registers */
    time_taken= (TMR1L | (TMR1H<<8))/58; /* convert duration into distance */
    __delay_ms(150);   
    
    return time_taken;
}
void setupPWM(void) {
   // Configuraci�n de Timer2 para generar PWM
    PR2 = 250;
    T2CONbits.T2CKPS = 0b11;
    T2CONbits.TMR2ON = 1;
    
    // Configuraci�n del CCP1 para salida PWM
    CCP1CONbits.CCP1M = 0b1100; // Modo PWM
    CCP1CONbits.DC1B = 0;       // Ajuste fino en 0
    
    // Configuraci�n de pines (RC2 como salida)
    TRISCbits.TRISC2 = 0;
}





