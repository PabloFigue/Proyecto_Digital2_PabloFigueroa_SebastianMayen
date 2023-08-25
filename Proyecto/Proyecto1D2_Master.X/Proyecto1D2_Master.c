/* 
 * File:   Proyecto1D2_Master.c
 * Author: Mayen / Figueroa
 *
 * Created on 15 de agosto de 2023, 09:20 PM
 */
//******************************************************************************
//Palabra de congifucion
//******************************************************************************
// CONFIG1
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
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include <xc.h>
#include "LCD_8bits.h"
#include "INTERRUPT_Lib.h"
#include "USART_Library.h"
#include "I2C.h"
#include "RTC.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
int8_t flag_parking = 0;
int8_t horariocar = 0,car =0,manual =0;
int z;
int distancia,var_distancia;
char distancia_string[10];
int sensor_Infra;
char sensor_Infra_string[10];
int sensor_Luz;
char sensor_Luz_string[10];
int8_t day, mth, year, dow, hr, min, sec;
char buffer[20];
char usart[20];
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    Lcd_Init();
    UART_TX_config(9600); //Configurar transmision y recepcion de datos por comunicacion serial a 9600 baudios.
    
    // MOSTRAR EN LCD DISTANCIA DEL SENSOR ULTRASONICO
    I2C_Master_Start();
    I2C_Master_Write(0x51);
    var_distancia = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(50); 
    // MOSTRAR EN LA LCD VALOR DE LUMINOSIDAD
    I2C_Master_Start();
    I2C_Master_Write(0x52);
    I2C_Master_Write(0b00000000);
    I2C_Master_Stop();
    __delay_ms(50);    

    I2C_Master_Start();
    I2C_Master_Write(0x53);
    sensor_Luz = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(50);
    //MOSTARA EN LA LCD VALOR DE INFRAROJO
    I2C_Master_Start();
    I2C_Master_Write(0x52);
    I2C_Master_Write(0b00000001);
    I2C_Master_Stop();
    __delay_ms(50);    

    I2C_Master_Start();
    I2C_Master_Write(0x53);
    sensor_Infra = I2C_Master_Read(0);
    I2C_Master_Stop();
    __delay_ms(50);    

    DS3231_Set_Date_Time(19,8,23,2,00,49,50);
    while(1){ 
        manual = PORTAbits.RA0;
        Lcd_Clear(); 
        // MOSTARA TITULOS EN LCD
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("DIST");
        Lcd_Set_Cursor(2,3);
        Lcd_Write_String("cm");  
        Lcd_Set_Cursor(1,7);
        Lcd_Write_String("LUZ");
        Lcd_Set_Cursor(1,12);
        Lcd_Write_String("INF");
        
        // MOSTRAR EN LCD DISTANCIA DEL SENSOR ULTRASONICO
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        var_distancia = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(50); 
        distancia = 11;
        
        if(var_distancia <=10){
            I2C_Master_Start();
            I2C_Master_Write(0x51);
            var_distancia = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(50);
            if (var_distancia <= 10){
                distancia = var_distancia;
            }
        }
        
        if(var_distancia >= 0 && var_distancia < 10){
            Lcd_Set_Cursor(2,2);
            Lcd_Write_String(" ");
        }
        if(var_distancia >= 99){     
           var_distancia = 99;
        }    
            Lcd_Set_Cursor(2,1);
            sprintf(distancia_string, "%d", var_distancia);
            Lcd_Write_String(distancia_string); 
                
        
        // MOSTRAR EN LA LCD VALOR DE LUMINOSIDAD
        I2C_Master_Start();
        I2C_Master_Write(0x52);
        I2C_Master_Write(0b00000000);
        I2C_Master_Stop();
        __delay_ms(50);    
       
        I2C_Master_Start();
        I2C_Master_Write(0x53);
        sensor_Luz = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(50);
        
        if (sensor_Luz <= 99){
            Lcd_Set_Cursor(2,7);
            sprintf(sensor_Luz_string, "0%d", sensor_Luz);
            Lcd_Write_String(sensor_Luz_string);
        }else{
            Lcd_Set_Cursor(2,7);
            sprintf(sensor_Luz_string, "%d", sensor_Luz);
            Lcd_Write_String(sensor_Luz_string);
        }
        
        //MOSTARA EN LA LCD VALOR DE INFRAROJO
        I2C_Master_Start();
        I2C_Master_Write(0x52);
        I2C_Master_Write(0b00000001);
        I2C_Master_Stop();
        __delay_ms(50);    
       
        I2C_Master_Start();
        I2C_Master_Write(0x53);
        sensor_Infra = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(50);
        
        Lcd_Set_Cursor(2,12);
        sprintf(sensor_Infra_string, "%d", sensor_Infra);
        Lcd_Write_String(sensor_Infra_string);
        
        //MOSTRAR FECHA Y HORA EN LCD
        if (sensor_Infra ==  1){
            if (horariocar == 0){
                Lcd_Clear();
                DS3231_Get_Date(&day, &mth, &year, &dow);
                DS3231_Get_Time(&hr, &min, &sec);

                sprintf(buffer, "%02u/%02u/20%02u", day, mth, year);
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String(buffer);
                sprintf(buffer, "%02u:%02u:%02u", hr, min, sec);
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String(buffer);
                
                sprintf(usart,"+%02u:%02u:%02u\r",hr, min, sec);
                UART_write_string(usart);    //Escribir un string en pantalla
                __delay_ms(2000);
                horariocar = 1;
            }
        }
        
///// ENVIAR DATOS DE LOS SENSORES A ESP32 POR USART ////      
       //UART_write_char(49);
        sprintf(usart, "%02u/%02u/%02u\n", var_distancia, sensor_Luz, sensor_Infra);
        UART_write_string(usart);    //Escribir un string en pantalla
        
/// PROGRAMACIÓN FUNCIONAMIENTO GARAGE ///////
        
        if (distancia <= 10 && manual ==0){   //Si la lectura del Ultrasonico es menor a 10

            if (car == 2){
                //Cerrar Servo
                I2C_Master_Start();
                I2C_Master_Write(0x50);
                I2C_Master_Write(0b00000000);
                I2C_Master_Stop();
                __delay_ms(50);  
                Lcd_Clear(); 
                Lcd_Write_String("PARQUEO LLENO");
                __delay_ms(3000);
               // PORTA = 0;
            }else if (car == 1){ //Si se detecta ya 1 carro adentro y quiere entrar otro
               // PORTA = 255;
                car = 2;
                //Codigo para Activar el DC - subir
                I2C_Master_Start();
                I2C_Master_Write(0x52);
                I2C_Master_Write(0b00000010);
                I2C_Master_Stop();
                __delay_ms(50); 
                
                I2C_Master_Start();
                I2C_Master_Write(0x53);
                z = I2C_Master_Read(0);
                I2C_Master_Stop();
                __delay_ms(50);
        
                while (sensor_Infra == 1){
                    I2C_Master_Start();
                    I2C_Master_Write(0x52);
                    I2C_Master_Write(0b00000001);
                    I2C_Master_Stop();
                    __delay_ms(50);    

                    I2C_Master_Start();
                    I2C_Master_Write(0x53);
                    sensor_Infra = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    __delay_ms(50);
                }
                //
                horariocar = 0;
                //Abrir Servo
                I2C_Master_Start();
                I2C_Master_Write(0x50);
                I2C_Master_Write(0b00000001);
                I2C_Master_Stop();
                __delay_ms(50);
                
                distancia = 10;
                sensor_Infra = 0;
                while (sensor_Infra == 0){  // como subio el primer carro el sensor deja de detectar por lo que hasta que no vualva a detectar el 2do no cierra
                    while (distancia <= 10){ // hasta que el carro no se entre y deje de detectarlo no sigue 
                       //
                       I2C_Master_Start();
                       I2C_Master_Write(0x51);
                       distancia = I2C_Master_Read(0);
                       I2C_Master_Stop();
                       __delay_ms(50);    
                    } 
                    
                    I2C_Master_Start();
                    I2C_Master_Write(0x52);
                    I2C_Master_Write(0b00000001);
                    I2C_Master_Stop();
                    __delay_ms(50);    
                    I2C_Master_Start();
                    I2C_Master_Write(0x53);
                    sensor_Infra = I2C_Master_Read(0);
                    I2C_Master_Stop();
                     __delay_ms(50);                       
                }
                
            }else{
                //Abrir Servo
                I2C_Master_Start();
                I2C_Master_Write(0x50);
                I2C_Master_Write(0b00000001);
                I2C_Master_Stop();
                __delay_ms(50);
            }                    
               
        }else if (distancia > 10 && sensor_Infra == 1 && car==0 && manual == 0){
            //Cerrar Servo
            I2C_Master_Start();
            I2C_Master_Write(0x50);
            I2C_Master_Write(0b00000000);
            I2C_Master_Stop();
            __delay_ms(50);  
            car =1;
        }else if (distancia > 10 && sensor_Infra == 1 && car==2 && manual == 0){
            //Cerrar Servo
            I2C_Master_Start();
            I2C_Master_Write(0x50);
            I2C_Master_Write(0b00000000);
            I2C_Master_Stop();
            __delay_ms(50);  
            
        }else if (manual == 1 && distancia > 10 && sensor_Infra == 1){
            //Abrir Servo
            __delay_ms(100);
            I2C_Master_Start();
            I2C_Master_Write(0x50);
            I2C_Master_Write(0b00000001);
            I2C_Master_Stop();
            __delay_ms(2000); 
        
            if (car == 2){
                while (distancia >10){
                    while (sensor_Infra == 1){
                        I2C_Master_Start();
                        I2C_Master_Write(0x52);
                        I2C_Master_Write(0b00000001);
                        I2C_Master_Stop();
                        __delay_ms(50);    

                        I2C_Master_Start();
                        I2C_Master_Write(0x53);
                        sensor_Infra = I2C_Master_Read(0);
                        I2C_Master_Stop();
                        __delay_ms(50);  
                    } 
                    I2C_Master_Start();
                    I2C_Master_Write(0x51);
                    distancia = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    __delay_ms(50); 
                }
                //Cerrar Servo
                I2C_Master_Start();
                I2C_Master_Write(0x50);
                I2C_Master_Write(0b00000000);
                I2C_Master_Stop();
                __delay_ms(50);
                //Codigo para Activar el DC - bajar
                I2C_Master_Start();
                I2C_Master_Write(0x52);
                I2C_Master_Write(0b00000011);
                I2C_Master_Stop();
                __delay_ms(50);    
                // 
                car = 1;
                        
            }else if (car == 1){
                while (distancia >10){
                    while (sensor_Infra == 1){
                        I2C_Master_Start();
                        I2C_Master_Write(0x52);
                        I2C_Master_Write(0b00000001);
                        I2C_Master_Stop();
                        __delay_ms(50);    

                        I2C_Master_Start();
                        I2C_Master_Write(0x53);
                        sensor_Infra = I2C_Master_Read(0);
                        I2C_Master_Stop();
                        __delay_ms(50);  
                    } 
                    I2C_Master_Start();
                    I2C_Master_Write(0x51);
                    distancia = I2C_Master_Read(0);
                    I2C_Master_Stop();
                    __delay_ms(50); 
                }
                 //Cerrar Servo
                I2C_Master_Start();
                I2C_Master_Write(0x50);
                I2C_Master_Write(0b00000000);
                I2C_Master_Stop();
                __delay_ms(50);
                car =0;
            }
        horariocar = 0;
        manual = 0;
        __delay_ms(2000);
        }
    }
    return;  
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0b1;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
    

    I2C_Master_Init(100000);        // Inicializar Comuncación I2C a 1k
    //INTERRUPCIÓN GLOBAL
    INTCONbits.GIE = 1;         // Activación Interrupción Global
    INTCONbits.PEIE = 1; // habilitar interrupciones perifericas
    
    
    OSCCONbits.IRCF = 0b111;  // Frecuencia de oscilacion interna 8MHz
    OSCCONbits.SCS = 1;
}

