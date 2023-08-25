#include "LCD_8bits.h"

void Lcd_Port(char a) {
    if (a & 1)
        D0 = 1;
    else
        D0 = 0;
    
    if (a & 2)
        D1 = 1;
    else
        D1 = 0;
    
    if (a & 4)
        D2 = 1;
    else
        D2 = 0;
    
    if (a & 8)
        D3 = 1;
    else
        D3 = 0;

    if (a & 16)
        D4 = 1;
    else
        D4 = 0;

    if (a & 32)
        D5 = 1;
    else
        D5 = 0;

    if (a & 64)
        D6 = 1;
    else
        D6 = 0;
    
    if (a & 128)
        D7 = 1;
    else
        D7 = 0;
    
    
}

void Lcd_Cmd(char a) {
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    EN = 0; // => E = 0
}

void Lcd_Clear(void) {
    Lcd_Cmd(0b01);
    __delay_ms(2);
}
void Lcd_Set_Cursor(char a, char b) {
    // Configura la posición del cursor en la pantalla
    char position;
    if (a == 1) {
        position = 0x80 + b - 1;
    } else if (a == 2) {
        position = 0xC0 + b - 1;
    }
    Lcd_Cmd(position); // Envía el comando para establecer la posición del cursor
}

void Lcd_Init(void) {
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x30);
    __delay_ms(5);
    Lcd_Cmd(0x030);
    __delay_ms(100);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x38);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x06);
    
}
void Lcd_Write_Char(char a) {
    RS = 1; // RS = 1 para enviar datos
    Lcd_Port(a); // Envía los 8 bits del carácter
    EN = 1; // E = 1 para habilitar la escritura
    __delay_us(1); // Pequeño retardo de pulso de habilitación
    EN = 0; // E = 0 para finalizar el pulso de habilitación
}
void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}


