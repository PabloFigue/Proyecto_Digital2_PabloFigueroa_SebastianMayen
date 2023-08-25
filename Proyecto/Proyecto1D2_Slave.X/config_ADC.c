#include "config_ADC.h"

void adc_init(int channel){
    // Configurar el canal del ADC y otras configuraciones según sea necesario
    adc_change_channel(channel);

    // Configurar el ADC
    ADCON1bits.ADFM = 0; // Justificación a la derecha (los 8 bits más significativos se almacenan en ADRESH)
    ADCON1bits.VCFG0 = 0; // Voltaje de referencia VSS
    ADCON1bits.VCFG1 = 0; // Voltaje de referencia VDD
    PIE1bits.ADIE = 1; // Habilitar la interrupción del ADC
    //PIR1bits.ADIF = 0; // Limpiar la bandera de interrupción del ADC
    
    ADCON0bits.ADCS = 0b10; // Fosc/8 (El tiempo de adquisición será 2TAD)
    ADCON0bits.ADON = 1; // Habilitar el ADC
}
int adc_read(){
    ADCON0bits.GO = 1; // Iniciar la conversión
    
    while (ADCON0bits.GO); // Esperar a que la conversión se complete
    
    // La conversión ha terminado, el resultado se encuentra en ADRESH y ADRESL (10 bits)
    // Sin embargo, solo estamos interesados en los 8 bits más significativos.
    return ADRESH;
}
void adc_change_channel(int channel){
    ADCON0bits.CHS = channel; // Cambiar el canal del ADC
}
int adc_get_channel(){
    return ADCON0bits.CHS; // Devolver el canal actual del ADC
}

