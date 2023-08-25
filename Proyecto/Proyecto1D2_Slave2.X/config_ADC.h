/* 
 * File:   config_ADC.h
 * Author: Mayen
 *
 * Created on 18 de agosto de 2023, 11:35 PM
 */

#ifndef CONFIG_ADC_H
#define	CONFIG_ADC_H

#include <xc.h>

void adc_init(int channel);
int adc_read();
void adc_change_channel(int channel);
int adc_get_channel();

#endif	/* CONFIG_ADC_H */

