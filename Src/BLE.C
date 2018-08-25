/*
	BLE related Source 
  Author: Mrunal Ahirrao
*/

#include "BLE.h"
#include <string.h>
#include "stm32f0xx_hal.h"

extern UART_HandleTypeDef huart1;


/*
	This function intializes BLE
*/

void BLE_Init(){
		
	if(!flag.BLE_PWRUP &&	!flag.BLE_PWRUP_COMM_SENT	&&	!flag.BLE_COMM_ONGOING){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)BLE_PWR_UP,strlen(BLE_PWR_UP),200);
		flag.BLE_COMM_ONGOING	=	SET;
		flag.BLE_PWRUP_COMM_SENT	=	SET;

	}else if(flag.BLE_PWRUP &&	flag.BLE_PWRUP_COMM_SENT	&&	!flag.BLE_COMM_ONGOING	&&	!flag.BLE_SPP_ENBLD 
	&& !flag.BLE_EN_SPP_COMM_SENT){

		HAL_UART_Transmit(&huart1,(unsigned char*)BLE_EN_SPP,strlen(BLE_EN_SPP),200);
		flag.BLE_COMM_ONGOING	=	SET;
		flag.BLE_EN_SPP_COMM_SENT	=	SET;
	}
			
}

/*
	This function disconnects BLE
*/
void BLE_Disconnect(){
	
	if(!flag.BLE_DISCON_COMM_SENT	&& flag.BLE_CONNECTED){
	
		HAL_UART_Transmit(&huart1,(unsigned char*)BLE_DEREG_SPP_SERVER,strlen((char*)BLE_DEREG_SPP_SERVER),200);
		flag.BLE_COMM_ONGOING	=	SET;
		flag.BLE_DISCON_COMM_SENT	=	SET;
	
	}
	
}

/*
	This function Powers down BLE
*/
void BLE_POWER_DN(){

	if(!flag.BLE_PWRDN_COMM_SENT	&& flag.BLE_PWRUP){
	
		HAL_UART_Transmit(&huart1,(unsigned char*)BLE_POWER_DN,strlen((char*)BLE_POWER_DN),200);
		flag.BLE_COMM_ONGOING	=	SET;
		flag.BLE_PWRDN_COMM_SENT	=	SET;
		
	}

}

/*
	This function sends indication to connected BLE Device
@param: message should be hex represented in ASCII like a=0x61 so for a we should send '61'
*/
void BLE_SEND_IND(unsigned char message[]){

	unsigned char BLE_COMMD[50];
	
	strncpy((char*)BLE_COMMD,"\0",strlen((char*)BLE_COMMD));//clear buffer
	
	if(!flag.BLE_SENDIND_COMM_SENT	&& flag.BLE_CONNECTED){
		
		strncat((char*)BLE_COMMD,BLE_SPP_SEND_IND,strlen(BLE_SPP_SEND_IND));
		strncat((char*)BLE_COMMD,(char*)message,strlen((char*)message));
		strncat((char*)BLE_COMMD,"\r\n",2);
		
		HAL_UART_Transmit(&huart1,(unsigned char*)BLE_COMMD,strlen((char*)BLE_COMMD),200);
		flag.BLE_COMM_ONGOING	=	SET;
		flag.BLE_SENDIND_COMM_SENT	=	SET;

	}

}

/*
	This fucntion resets all BLE flags
*/
void BLE_ResetFlags(void){
	
		flag.BLE_GATT_REGSRD	=	SET;
		flag.BLE_COMM_ONGOING	=	RESET;
		flag.BLE_EN_SPP_COMM_SENT	=	RESET;
		flag.BLE_GATT_REG_COMM_SENT	=	RESET;
		flag.BLE_PWRUP_COMM_SENT	=	RESET;			

}
