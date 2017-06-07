#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "usart.h"



void set_bt_buffer_bit(uint8_t, bool);	//������ �������� ���� � ������ ��� ��-������, ��������� �� ������
trx_event parsing_bt_data(void);//����������� �����, ���������� �� ������, �������� �� ���� ������
void test_bt_data(void);//������������ �������� ����� ������
bool get_bt_buffer_bit(uint8_t);	//��������� �������� ���� � ������ ������, ��������� �� ������
trx_packet get_bt_packet_value(void); //��������� ������ �� ����������� ������
tir_message get_bt_message_value(void); //��������� ��������� �� ����������� ������
//extern volatile uint8_t bt_rx_buffer[RX_BLUETOOTH_BUFFER_SIZE]; 	//������ ��� ��-������, ��������� �� ������

#endif /* __BLUETOOTH_H */
