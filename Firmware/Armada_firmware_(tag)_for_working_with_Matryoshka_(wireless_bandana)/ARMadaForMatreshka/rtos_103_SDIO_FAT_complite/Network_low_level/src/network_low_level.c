#include  "network_low_level.h"
#include "network_base_types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "global_variables.h"
#include <string.h>
#include <stdint.h>
#include "types.h"

#ifdef SI4432_ENABLE

//���������� �����
volatile tWaitingPackage rf_TxBuf[RF_TX_BUFFER_SIZE];

//����� ������� �������� �������

volatile tLastReceivedPackagesItem rf_lastRxBuff[RF_LAST_RECEIVED_PACKAGES_BUFFER_SIZE];

volatile unsigned char rf_txBufTail = 0;
volatile unsigned char rf_txBufHead = 0;
volatile unsigned char rf_txCount = 0;

volatile unsigned char rf_lastRxBufTail = 0;
volatile unsigned char rf_lastRxBufHead = 0;
volatile unsigned char rf_lastRxCount = 0;

/*
//������� �����
volatile tPackage rf_RxBuf[RF_RX_BUFFER_SIZE];

volatile unsigned char rf_rxBufTail = 0;
volatile unsigned char rf_rxBufHead = 0;
volatile unsigned char rf_rxCount = 0;
*/


uint16_t rf_generatePackageId()
{
static volatile uint16_t last_ID = 3;
	if (last_ID==0xFFFF) last_ID = 3;
	return last_ID++;
}


unsigned char rf_GetTxCount(void)
{
  return  rf_txCount;
}


unsigned char rf_GetLastRxCount(void){
	return rf_lastRxCount;
}



/*
unsigned char rf_GetRxCount(void)
{
  return  rf_rxCount;
}
*/

//"�������" ���������� �����
void rf_FlushTxBuf(void)
{
	rf_txBufTail = 0;
	rf_txBufHead = 0;
	rf_txCount = 0;
}


void rf_FlushLastRxBuf(void)
{
	rf_lastRxBufTail=0;
	rf_lastRxBufHead=0;
	rf_lastRxCount=0;
}


/*
//"�������" ������� �����
void rf_FlushRxBuf(void)
{
	rf_rxBufTail = 0;
	rf_rxBufHead = 0;
	rf_rxCount = 0;
}
*/
bool rf_PutLastRxPackage(tLastReceivedPackagesItem new_package) //�������� ghbyznsq ����� � �����
{

	 if (rf_lastRxCount < RF_LAST_RECEIVED_PACKAGES_BUFFER_SIZE){    //���� � ������ ��� ���� �����
		 rf_lastRxBuff[rf_lastRxBufTail] = new_package; //�������� � ���� ������
		 rf_lastRxCount++;                   //�������������� ������� ��������
		 rf_lastRxBufTail++;                 //� ������ ������ ������
		 if (rf_lastRxBufTail == RF_LAST_RECEIVED_PACKAGES_BUFFER_SIZE) rf_lastRxBufTail = 0;
		 return true;
		 }
	 return false;
}



bool rf_PutWaitingPackage(tWaitingPackage new_package) //�������� ����� � �����
{

		 if (rf_txCount < RF_TX_BUFFER_SIZE){    //���� � ������ ��� ���� �����
			 rf_TxBuf[rf_txBufTail] = new_package; //�������� � ���� ������
			 rf_txCount++;                   //�������������� ������� ��������
			 rf_txBufTail++;                 //� ������ ������ ������
			 if (rf_txBufTail == RF_TX_BUFFER_SIZE) rf_txBufTail = 0;
			 return true;
			 }
		 return false;
}



//������ ������
bool rf_GetWaitingPackage(tWaitingPackage* package_tmp)
{
	//tWaitingPackage package_tmp;
  if (rf_txCount > 0){                     //���� �������� ����� �� ������
	 *package_tmp = rf_TxBuf[rf_txBufHead];        //��������� �� ���� ������
    rf_txCount--;                          //��������� ������� ��������
    rf_txBufHead++;                        //���������������� ������ ������ ������
    if (rf_txBufHead == RF_TX_BUFFER_SIZE) rf_txBufHead = 0;
    return true;                         //������� ����������� ������
  }
  return false;
}


bool rf_GetLastRxPackage(tLastReceivedPackagesItem* package_tmp){
	//tWaitingPackage package_tmp;
  if (rf_lastRxCount > 0){                     //���� �������� ����� �� ������
	 *package_tmp = rf_lastRxBuff[rf_lastRxBufHead];        //��������� �� ���� ������
    rf_lastRxCount--;                          //��������� ������� ��������
    rf_lastRxBufHead++;                        //���������������� ������ ������ ������
    if (rf_lastRxBufHead == RF_LAST_RECEIVED_PACKAGES_BUFFER_SIZE) rf_lastRxBufHead = 0;
    return true;                         //������� ����������� ������
  }
  return false;

}

void rf_InitDefaultPackageTimings(tPackageTimings* pPackageTimings){

	pPackageTimings->timeout = (defaultTimeout/1000000)*TIC_FQR;
	pPackageTimings->resendTime = (defaultResendTime*TIC_FQR)/1000000;
	pPackageTimings->resendTimeDelta = (defaultResendTimeDelta*TIC_FQR)/1000000;
	pPackageTimings->infiniteResend = false;

}


extern const tDeviceAddress my_rf_net_address;

void rf_InitDefaultWaitingPackage(tWaitingPackage* waiting_package_tmp){

	 uint32_t time_tmp;
	 time_tmp =  xTaskGetTickCount();
	 rf_InitDefaultPackageTimings(&waiting_package_tmp->timings);
	 waiting_package_tmp->package.details.packageId = rf_generatePackageId();
	 waiting_package_tmp->package.details.needAck = 1;

	 waiting_package_tmp->wasCreated =time_tmp;
	 waiting_package_tmp->nextTransmission = time_tmp;
	 waiting_package_tmp->isBroadcast = true;
	 waiting_package_tmp->callback =  '\0'/*rf_txDoneCallback*/;
	 waiting_package_tmp->package.sender=my_rf_net_address;
}


void rf_txDoneCallback(uint16_t tx_done_packageId){


}

/*
#define MAXSTACK 256//1024//2048		// ������������ ������ �����


static volatile long lbstack[MAXSTACK], ubstack[MAXSTACK]; // ���� ��������
                       // ������ ������ �������� ����� ��������,
                       // � ������: �����(lbstack) � ������(ubstack)
                       // ��������� ����������



void rf_SortTxBuffByNextTransmission (void){

	long size;
	size = rf_GetTxCount();

    long i, j;   			// ���������, ����������� � ����������

      long lb, ub;  		// ������� ������������ � ����� ���������

      long stackpos = 1;   	// ������� ������� �����
      long ppos;            // �������� �������
      tWaitingPackage pivot;              // ������� �������
      tWaitingPackage temp;
      lbstack[1] = 0;
      ubstack[1] = size-1;

      do {

         // ����� ������� lb � ub �������� ������� �� �����.

         lb = lbstack[ stackpos ];
         ub = ubstack[ stackpos ];
         stackpos--;

         do {
           // ��� 1. ���������� �� �������� pivot

           ppos = ( lb + ub ) >> 1;
           i = lb; j = ub; pivot = rf_TxBuf[ppos];

           do {
             while ( rf_TxBuf[i].nextTransmission < pivot.nextTransmission ) i++;
             while ( pivot.nextTransmission < rf_TxBuf[j].nextTransmission ) j--;

             if ( i <= j ) {
               temp = rf_TxBuf[i]; rf_TxBuf[i] = rf_TxBuf[j]; rf_TxBuf[j] = temp;
               i++; j--;
             }
           } while ( i <= j );

           // ������ ��������� i ��������� �� ������ ������� ����������,
           // j - �� ����� ������ (��. ����������� ����), lb ? j ? i ? ub.
           // �������� ������, ����� ��������� i ��� j ������� �� ������� �������

           // ���� 2, 3. ���������� ������� ����� � ����  � ������� lb,ub

           if ( i < ppos ) {     // ������ ����� ������

             if ( i < ub ) {     //  ���� � ��� ������ 1 �������� - �����
               stackpos++;       //  �����������, ������ � ����
               lbstack[ stackpos ] = i;
               ubstack[ stackpos ] = ub;
             }
             ub = j;             //  ��������� �������� ����������
                                 //  ����� �������� � ����� ������

           } else {       	    // ����� ����� ������

             if ( j > lb ) {
               stackpos++;
               lbstack[ stackpos ] = lb;
               ubstack[ stackpos ] = j;
             }
             lb = i;
           }

         } while ( lb < ub );        // ���� � ������� ����� ����� 1 ��������

       } while ( stackpos != 0 );    // ���� ���� ������� � �����
}
*/

bool rf_send(tDeviceAddress target, tDeviceAddress sender,  uint8_t* data, uint16_t size,  bool waitForAck,  tPackageTimings timings){
	bool result=false;
	tWaitingPackage rf_package_tmp;
	rf_package_tmp.package.target = target;
	rf_package_tmp.package.sender = sender;
	rf_package_tmp.package.details.needAck = waitForAck;
	rf_package_tmp.timings = timings;
	memcpy (rf_package_tmp.package.payload, data, size);
	 if (size<payloadLength)
	        {
	            memset(&(rf_package_tmp.package.payload[size]), 0, payloadLength-size);
	}
	 rf_package_tmp.package.details.packageId = rf_generatePackageId();
	 rf_package_tmp.isDeliveryComplite = false;
	 rf_package_tmp.wasCreated = xTaskGetTickCount();
	 rf_package_tmp.nextTransmission =  rf_package_tmp.wasCreated;
	 if (xSemaphoreTake(rf_tx_buff_Semaphore, TIC_FQR*2/*portMAX_DELAY*/ )== pdTRUE)//���� 2 ������� ���������� Si4432
	{


		result=rf_PutWaitingPackage(rf_package_tmp);
//		if (rf_GetTxCount()>1) 	rf_SortTxBuffByNextTransmission(); //������������� �� ������� ��������
		xSemaphoreGive(rf_tx_buff_Semaphore);
		xSemaphoreGive( Si4432_IQR_Semaphore);
		return result;
	}
	else result =  false;//�� ��������� ��������

return result;

}

bool rf_put_package_to_buffer(tDeviceAddress target, tDeviceAddress sender,  uint8_t* data, uint16_t size,  bool waitForAck,  tPackageTimings timings){
	bool result=false;
		tWaitingPackage rf_package_tmp;
		rf_package_tmp.package.target = target;
		rf_package_tmp.package.sender = sender;
		rf_package_tmp.package.details.needAck = waitForAck;
		rf_package_tmp.timings = timings;
		memcpy (rf_package_tmp.package.payload, data, size);
		 if (size<payloadLength)
		        {
		            memset(&(rf_package_tmp.package.payload[size]), 0, payloadLength-size);
		}
		 rf_package_tmp.package.details.packageId = rf_generatePackageId();
		 rf_package_tmp.isDeliveryComplite = false;
		 rf_package_tmp.wasCreated = xTaskGetTickCount();
		 rf_package_tmp.nextTransmission =  rf_package_tmp.wasCreated;



			result=rf_PutWaitingPackage(rf_package_tmp);
//			if (rf_GetTxCount()>1) 	rf_SortTxBuffByNextTransmission(); //������������� �� ������� ��������
			return result;


	return result;
}

bool rf_isLastPackageInBuf(tPackage lastPackage)
{
	bool result = false;
	if(rf_GetLastRxCount())//���� ����� �� ������
	{

		if(rf_lastRxBufTail > rf_lastRxBufHead){
			for (unsigned char i = rf_lastRxBufHead; i < rf_lastRxBufTail; i++)
			{
				if (rf_lastRxBuff[i].id ==lastPackage.details.packageId)//���� ID ������ ���������
				{
					//� ���� ��������� ������
					if ((rf_lastRxBuff[i].sender.address[0]==lastPackage.sender.address[0])&&(rf_lastRxBuff[i].sender.address[1]==lastPackage.sender.address[1])&&(rf_lastRxBuff[i].sender.address[2]==lastPackage.sender.address[2]))
					{
						result=true;
					}
				}
			}



		}
		else
		{

			for (unsigned char i = rf_lastRxBufHead; i < RF_LAST_RECEIVED_PACKAGES_BUFFER_SIZE; i++)
			{
				if (rf_lastRxBuff[i].id==lastPackage.details.packageId)//���� ID ������ ���������
				{
					//� ���� ��������� ������
					if ((rf_lastRxBuff[i].sender.address[0]==lastPackage.sender.address[0])&&(rf_lastRxBuff[i].sender.address[1]==lastPackage.sender.address[1])&&(rf_lastRxBuff[i].sender.address[2]==lastPackage.sender.address[2]))
					{
						result = true;
					}
				}
			}
			for (unsigned char i = 0; i < rf_lastRxBufTail; i++)
			{
				if (rf_lastRxBuff[i].id==lastPackage.details.packageId)//���� ID ������ ���������
				{
					//� ���� ��������� ������
					if ((rf_lastRxBuff[i].sender.address[0]==lastPackage.sender.address[0])&&(rf_lastRxBuff[i].sender.address[1]==lastPackage.sender.address[1])&&(rf_lastRxBuff[i].sender.address[2]==lastPackage.sender.address[2]))
					{
						result = true;
					}
				}
			}
		}
	}

return result;

}



void rf_set_delivery_complite_flag(tPackageId package_id, tDeviceAddress target_address){

	if (xSemaphoreTake(rf_tx_buff_Semaphore, TIC_FQR*2/*portMAX_DELAY*/ )== pdTRUE)//���� 2 ������� ���������� Si4432
	{


		if(rf_GetTxCount())
		{
			if(rf_txBufTail>rf_txBufHead)
			{
				for (unsigned char i = rf_txBufHead; i < rf_txBufTail; i++)
				{
					if (rf_TxBuf[i].package.details.packageId ==package_id)//���� ID ������ ���������
					{
						//� ���� ��������� ������
						if ((rf_TxBuf[i].package.target.address[0]==target_address.address[0])&&(rf_TxBuf[i].package.target.address[1]==target_address.address[1])&&(rf_TxBuf[i].package.target.address[2]==target_address.address[2]))
						{
							rf_TxBuf[i].isDeliveryComplite=true;
						}
					}
				}
			}
			else
			{
				for (unsigned char i = rf_txBufHead; i < RF_TX_BUFFER_SIZE; i++)
				{
					if (rf_TxBuf[i].package.details.packageId ==package_id)//���� ID ������ ���������
					{
						//� ���� ��������� ������
						if ((rf_TxBuf[i].package.target.address[0]==target_address.address[0])&&(rf_TxBuf[i].package.target.address[1]==target_address.address[1])&&(rf_TxBuf[i].package.target.address[2]==target_address.address[2]))
						{
							rf_TxBuf[i].isDeliveryComplite=true;
						}
					}
				}
				for (unsigned char i = 0; i < rf_txBufTail; i++)
				{
					if (rf_TxBuf[i].package.details.packageId ==package_id)//���� ID ������ ���������
					{
						//� ���� ��������� ������
						if ((rf_TxBuf[i].package.target.address[0]==target_address.address[0])&&(rf_TxBuf[i].package.target.address[1]==target_address.address[1])&&(rf_TxBuf[i].package.target.address[2]==target_address.address[2]))
						{
							rf_TxBuf[i].isDeliveryComplite=true;
						}
					}
				}
			}
		}



		xSemaphoreGive(rf_tx_buff_Semaphore);
	}

}


#endif //[#ifdef SI4432_ENABLE]
