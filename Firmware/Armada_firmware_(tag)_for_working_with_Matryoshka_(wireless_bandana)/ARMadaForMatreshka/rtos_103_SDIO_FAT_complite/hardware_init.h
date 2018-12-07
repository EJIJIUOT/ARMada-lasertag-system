#ifndef __HARDWARE_INIT_H
#define __HARDWARE_INIT_H

#include "stm32f10x.h"
#include "miles_protocol.h"
#include "global_variables.h"
#include "types.h"

void init_spi2(void);//��������� SPI2
//������ 6 ������ ������� ������ ���, ����� ��� ������ ��������� �������� ������
void init_timer(uint32_t sampleRate);
void init_dac(void);
void DMA_for_ADC_Init(void);
void init_adc_for_multiscan(void);//��������� ��� ��� ����������������� ������������ �������
void init_adc(void);//��������� ���
void init_pwm_TIM4(void);
void init_pwm_TIM3(void);
void init_pwm(void);
void init_pwm_TIM2(void);
void init_pwm_TIM5(void);
void init_TIM7(void);
void init_pwm_TIM8(void);





#endif
