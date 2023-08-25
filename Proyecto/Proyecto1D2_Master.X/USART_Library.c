#include <xc.h>
#include <string.h>

// Definiciones de velocidad de baudrate
#define FREQ 8000000

unsigned char contador = 0;

void UART_RX_config(uint16_t baudrate){
    // C�lculo del valor para el registro SPBRG y SPBRGH
    uint16_t baud_value = ((FREQ / baudrate)/64)-1;
    //SPBRGH:SPBRG = (8MHz/9600)/64 -1 = 12 ---> Real 9615.38
    //%error = (9615.38-9600)/9600*100 = 0.16%

    // Asignar el valor a los registros SPBRG y SPBRGH
    SPBRG = baud_value & 0xFF;       // Byte menos significativo
    SPBRGH = (baud_value >> 8) & 0xFF; // Byte m�s significativo

    // Configuraci�n de bits del registro TXSTA
    TXSTAbits.BRGH = 0; // Modo de baja velocidad
    BAUDCTLbits.BRG16 = 0;
    TXSTAbits.SYNC = 0; // Modo as�ncrono

    // Configuraci�n de bits del registro RCSTA
    RCSTAbits.SPEN = 1; // Habilitar el UART
    TXSTAbits.TXEN = 1; // Habilitar transmisi�n
    RCSTAbits.CREN = 1; // Habilitar recepci�n continua
}

// Funci�n de configuraci�n del UART para transmisi�n
void UART_TX_config(uint16_t baudrate) {
    // C�lculo del valor para el registro SPBRG y SPBRGH
    uint16_t baud_value = ((FREQ / baudrate)/64)-1;
    //SPBRGH:SPBRG = (8MHz/9600)/64 -1 = 12 ---> Real 9615.38
    //%error = (9615.38-9600)/9600*100 = 0.16%

    // Asignar el valor a los registros SPBRG y SPBRGH
    SPBRG = baud_value & 0xFF;       // Byte menos significativo
    SPBRGH = (baud_value >> 8) & 0xFF; // Byte m�s significativo

    // Configuraci�n de bits del registro TXSTA
    TXSTAbits.BRGH = 0; // Modo de baja velocidad
    BAUDCTLbits.BRG16 = 0;
    TXSTAbits.SYNC = 0; // Modo as�ncrono

    // Configuraci�n de bits del registro RCSTA
    RCSTAbits.SPEN = 1; // Habilitar el UART
    TXSTAbits.TXEN = 1; // Habilitar transmisi�n
}

// Funci�n para escribir un car�cter en el UART
void UART_write_char(char c) {
    while (!TXIF);  // Esperar hasta que el buffer de transmisi�n est� vac�o
    TXREG = c;      // Cargar el car�cter en el buffer de transmisi�n
}

// Funci�n para leer un car�cter del UART
char UART_read_char() {
    if (RCREG == '+'){      //Si el caracter es +, entonces suma 1 a la variable contador
        contador = contador+1;
    }else if (RCREG == '-'){    //Si el caracter es -, entonces resta 1 a la variable contador.
        contador =contador-1;
    }
    return contador;   // Retornar el car�cter recibido
}

// Funci�n para enviar una cadena de caracteres por el UART
void UART_write_string(const char *str) {
    while (*str != '\0') {
        UART_write_char(*str); // Enviar un car�cter de la cadena
        str++; // Avanzar al siguiente car�cter de la cadena
    }
}

char UART_set_value(int byte_value){
    int centenas = (byte_value / 100 + 48);       // Calcular las centenas 
    int decenas = ((byte_value%100)/10)+48;    // Calcular las decenas 
    int unidades = (byte_value%10)+48;         // Calcular las unidades
    //Enviar cada uno de los valores.
    UART_write_char(centenas);
    UART_write_char('.');   //Escribir un punto para mostrar de 0.00 a 5.00 volts
    UART_write_char(decenas);
    UART_write_char(unidades);
}
