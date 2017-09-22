#ifndef __ARMADA_H
#define __ARMADA_H




#include "stm32f10x.h"
#include "usart.h"
#include "bluetooth.h"
// ���������� FreeRTOS
#include "FreeRTOS.h"
// �� ���� ������������ ���������� ���� ������ ������������ �����
#include "task.h"
#include "SDIO_SD/sdio_sd.h"
#include "ff.h"
#include <stdio.h>
#include <string.h>
#include "types.h"
//#include "miles_protocol.h"
#include "semphr.h"
#include "queue.h"
#include "slots.h"
#include "global_variables.h"
#include "hardware_init.h"
#include "cipher.h"
#include "menu.h"
#include <GFXC.h>


#include "display.h"
#include "game.h"

#define HIT_PROCESSING_TIMEOUT (TIC_FQR/2) //���������� �� ��������� ���������
#define USART_PROCESSING_TIMEOUT (TIC_FQR) //������� �� �������� ������

#define pi  3.14159
#define SHORT_DURATION 4 	//����������� ������������ (� "�����") ������������ ������� �����,
							//����������� ��� �������� ������� "����� �����"


typedef struct rx_hit{
portTickType time_stamp;
trx_packet rx_package;
} trx_hit;

typedef struct rx_captured_message{
portTickType time_stamp;
tir_message rx_message;
} trx_captured_message;

#define LCD_BL_OFF GPIO_SetBits(GPIOC, GPIO_Pin_13)//��������� ��������� ���
#define LCD_BL_ON  GPIO_ResetBits(GPIOC, GPIO_Pin_13)//�������� ��������� ���

#define compareHits(hit1, hit2) ((((hit1.time_stamp-10) <= hit2.time_stamp) && ((hit1.time_stamp+10) >= hit2.time_stamp))&&(hit1.rx_package.player_id == hit2.rx_package.player_id)&&(hit1.rx_package.team_id == hit2.rx_package.team_id)&&(hit1.rx_package.damage == hit2.rx_package.damage))//����� ��� ��� �� �������, �� ��������������� ������ �����?
#define compareMessages(mess1, mess2) ((mess1.time_stamp == mess2.time_stamp)&&(mess1.rx_message.ID == mess2.rx_message.ID)&&(mess1.rx_message.param == mess2.rx_message.param)&&(mess1.rx_message.control_byte == mess2.rx_message.control_byte))//����� ��� ��� �� �������, �� ��������������� ������ �����?
//���������� �������� ������� ����������, ���� �� ������� ������� ������-���� � SD-�����
#define DEFAULT_HEALTH 			100//0 			//������� �������� �����
#define DEFAULT_TEAM_COLOR 		Yellow//Red			//���� ������� �� ���������
#define DEFAULT_PLAYER_ID 		0
#define DEFAULT_GUN_DAMAGE 		Damage_50
#define DEFAULT_CLIPS 			15
#define DEFAULT_ROUNDS_IN_CLIP 	10

#define DEFAULT_PLAYER_STATUS 	IN_GAME//OUT_OF_GAME
#define DEFAULT_FRIENDLY_FIRE 	true//false
#define DEFAULT_SHOCK_TIME 	100//����� ���� �� ��������� - 1 �
#define DEFAULT_RELOAD_TIME 100//���� ������� ����� ����������� �� ���������
#define DEFAULT_RATE 300//���������������� �� ��������� (��������� � ������)
#define DEFAULT_BATTARY_FULL_VOLTAGE 4200 //���������� ��������� ���������� ������� �� ���������
#define DEFAULT_BATTARY_LOW_VOLTAGE 3400 //���������� ��������� ����������� ������� �� ���������
#define DEFAULT_IR_POWER 30 //�������� �� ��������� �� ���������
#define DEFAULT_IR_POWER_OFFSET 0 //�������� �� ������������ ���������
#define DEFAULT_AUTOSTART_GAME true //��������� ���� ����� ������ �������
#define DEFAULT_BACKLIGHT_LEVEL 2


//�������
/*************************************************************************/
void BluetoothTask(void *pvParameters);
void vTaskLED1(void *pvParameters);
void vTaskLED2(void *pvParameters);
void Wav_Player(void *pvParameters);
void Wav_Player_Manager(void *pvParameters);
void Zone4task(void *pvParameters);
void Zone3task(void *pvParameters);
void Zone2task(void *pvParameters);
void Zone1task(void *pvParameters);


#ifdef SI4432_ENABLE
void Si4432task(void *pvParameters);
//void Si4432_Init(void);
void Si4432_Rx_Tx_Init(void);
bool Si4432_TxBuf(unsigned char *pBuf, unsigned char bytes);
bool  Si4432_Rx(unsigned char *pBuf, unsigned char* pBytes, portTickType timeout);
#endif


#ifdef DIRECTION_CALCULATION
void DirectionCalculanionTask(void *pvParameters);
#endif
void init_gpio(void);
void init_timer(uint32_t sampleRate);//����������� ������
void init_dac(void);//����������� ���
void init_dma( uint16_t bitsPerSample);
void init_pwm(void);
void init_pwm_TIM3(void);
void init_pwm_TIM2(void);
void init_pwm_TIM4(void);
void init_pwm_TIM5(void);
void init_TIM7(void);
//void init_pwm_TIM8(void);
char wave_playback(const volatile char*);
tfire_mode test_fire_mode_switch(void);

void init_var(void);//����������� ���������� �������� �� ���������
void ir_tx_cursor_home(void);//������������� ������ � ������ ������

bool get_buffer_bit(uint8_t index);		//��������� �������� ���� � ������ ��-���������

bool get_zone_buffer_bit(TDamageZone, uint8_t index);		//��������� �������� ���� � ������ ��-���������
void set_buffer_bit(uint8_t, bool);	//������ �������� ���� � ������ ��-���������
void set_zone_buffer_bit (TDamageZone, uint8_t, bool);
void send_ir_shot_package(void); //���������� ����� ("��������")
void set_player_id(uint8_t);		//������ ������ �������������
void set_team_color(tteam_color);	//������ ���� ����� �������
void set_gun_damage(tgun_damage);	//������ ��������� ������ ������ (����)
void init_adc(void);//��������� ���
void init_adc_for_multiscan(void);//��������� ��� ��� ����������������� ������������ �������
void DMA_for_ADC_Init(void);//��������� DMA ��� ������ � ADC
void SPISend(uint16_t);//�������� ������ �� SPI2
void init_spi2(void);//��������� SPI2
void init_dma_to_spi2(void);//����������� DMA ��� ������ � SPI2 (���������)
void startSPI2(void);
bool stopWavPlayer(void);
void hit_processing(TDamageZone);//������������ ���������;
void bt_hit_processing(trx_packet);//������������ ���������, �������� �� ������
void message_processing(TDamageZone);//������������ ��������� (������� ������)
bool get_settings_from_ini_file(void);//��������� ������� ��  ini-�����
void parsing_string(char* record);//������ ������
uint8_t get_parameter_index(char* record);//��������, ��� �� ��������
trx_packet get_packet_value(TDamageZone); //��������� ������ �� ����������� ������

TKEYBOARD_STATUS get_keyboard_status(int adc_value); //���������, ����� �� �����
TKEYBOARD_EVENT test_keyboard(int adc_value);//��������� ������� ����������

TKEYBOARD_STATUS get_reload_key_status(int adc_value);//���������, ������ �� ������� "������������"
TKEYBOARD_EVENT test_reload_key(int adc_value);//��������� ������� ������� "������������"
bool save_parameters_to_sd_card(void);
bool read_parameters_from_sd_card(void);
bool send_set_at_command(char* cmd, char* param);
bool send_test_at_command(void);
void send_package_by_bluetooth(trx_packet package);
void send_message_by_bluetooth(tir_message message);
void configure_bluetooth(void);//����������� ������ ������
volatile  trx_hit hit_in_processing;//�������������� � ������ ������ �����
volatile trx_captured_message message_in_processing;
//char* int_to_str(uint8_t x, uint8_t digits);
//char* long_int_to_str(uint16_t x, uint8_t digits);
void bt_set_at_commands_mode(bool mode);//��������� ������ ������ � ����� at-������
void bt_reset(void);//���������� ����� ������-������
uint8_t get_damage_index(uint8_t damage);
void copy_to_lcd_buffer(char* src);
void full_screen(char value);
void clear_screen(void);
tir_message get_ir_message_from_buffer(TDamageZone zone);
//void Sinus_calculate(void);

//bool get_bt_buffer_bit(uint8_t);	//��������� �������� ���� � ������ ������, ��������� �� ������

void lock_firmware(void);
void BKPinit(void);

void RTC_Init(void);


uint32_t FtimeToCounter(ftime_t * ftime);
void CounterToFtime(uint32_t counter,ftime_t * ftime);
void getDateClockSettings (void);
void go_to_next_menu_item (uint8_t* item);
//void initGUI(void);//����� �� ����� ��������� ����������������� ����������
uint8_t checksum_for_clone_data(uint8_t*);
uint8_t checksum_for_tag_init_data(ttag_init_data);
//void bt_tag_init(void);
#if DEVICE_ROLE==BANDANA
void bt_tag_init_with_game_status(bool status, ttag_init_command);
#endif

#ifdef SENSORS_BECKLIGHT
void set_sensor_color(uint8_t color, uint8_t brightness_level, TDamageZone zone);
void set_sensor_vibro(uint8_t power_level, TDamageZone zone);
#endif





#endif /* __ARMADA_H */
