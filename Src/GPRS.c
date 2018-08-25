/*
This file contains the functions for GPRS connectivity

Author: Mrunal Ahirrao

*/

#include "GPRS.h"
#include "stm32f0xx_hal.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

/*
This function will intialize GPRS and connect with remote server

The operation of this function is designed in such a way that, each command will be executed one by one due to flags states
*/

void GPRS_init(){
	
	 unsigned char GPRSCOMM[50];
	
	if(!flag.GPRS_COMMAND_ONGOING	&& !flag.GPRS_AVLBLE){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)CHECK_GPRS_STAT,strlen(CHECK_GPRS_STAT),200);
		flag.GPRS_COMMAND_ONGOING	=	SET;
		
	}else if(!flag.GPRS_COMMAND_ONGOING	&&	flag.GPRS_AVLBLE	&&	!flag.OLD_CONN_SHUTOK){
	
		HAL_UART_Transmit(&huart1,(unsigned char*)RESET_IP_SESSION,strlen(RESET_IP_SESSION),200);
		flag.GPRS_COMMAND_ONGOING	=	SET;
		
	}else if(!flag.GPRS_COMMAND_ONGOING	&&	flag.OLD_CONN_SHUTOK	&&	flag.GPRS_AVLBLE	&&	!flag.TCP_STACK_INITOK){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)CHK_CIP_STAT,strlen(CHK_CIP_STAT),200);
		flag.GPRS_COMMAND_ONGOING	=	SET;
		
	}else if(!flag.GPRS_COMMAND_ONGOING	&&	flag.OLD_CONN_SHUTOK	&&	flag.GPRS_AVLBLE	&&	flag.TCP_STACK_INITOK	&&	!flag.SINGLECON_COMM_SENT){
	
		HAL_UART_Transmit(&huart1,(unsigned char*)SET_SINGLE_CONN,strlen(SET_SINGLE_CONN),200);
		flag.SINGLECON_COMM_SENT	=	SET;
		flag.GPRS_COMMAND_ONGOING	=	SET;
		
	}else if(!flag.GPRS_COMMAND_ONGOING	&&	flag.OLD_CONN_SHUTOK	&&	flag.GPRS_AVLBLE	&&	flag.TCP_STACK_INITOK	
	&&	flag.SINGLECON_COMM_OK_RCVD	&&	!flag.SETAPN_COMM_SENT){
		
		for(char i=0;i<strlen((char*)GPRSCOMM);i++){
			GPRSCOMM[i]	=	0;//clean up buffer
		}
		
		strcat((char*)GPRSCOMM,SET_APN);
		strcat((char*)GPRSCOMM,(char*)"\"");
		strcat((char*)GPRSCOMM,(char*)GPRS_Details.APN);
		strcat((char*)GPRSCOMM,(char*)"\"");
		strcat((char*)GPRSCOMM,(char*)"\r\n");
		
		HAL_UART_Transmit(&huart1,GPRSCOMM,strlen((char*)GPRSCOMM),200);
		flag.SETAPN_COMM_SENT	=	SET;
		flag.GPRS_COMMAND_ONGOING	=	SET;
		
	}else if(!flag.GPRS_COMMAND_ONGOING	&&	flag.OLD_CONN_SHUTOK	&&	flag.GPRS_AVLBLE	&&	flag.TCP_STACK_INITOK	
	&&	flag.SINGLECON_COMM_OK_RCVD	&&	flag.SETAPN_OK_RCVD		&&	!flag.GPRSUP_COMM_SENT){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)BRINGUP_WIRELESS_CONN,strlen((char*)BRINGUP_WIRELESS_CONN),200);
		flag.GPRSUP_COMM_SENT	=	SET;
		flag.GPRS_COMMAND_ONGOING	=	SET;
		
	}else if(!flag.GPRS_COMMAND_ONGOING	&&	flag.OLD_CONN_SHUTOK	&&	flag.GPRS_AVLBLE	&&	flag.TCP_STACK_INITOK	
	&&	flag.SINGLECON_COMM_OK_RCVD	&&	flag.SETAPN_OK_RCVD	&& flag.GPRSUP_OK_RCVD	&& !flag.GET_LOCAL_IP_COMM_SENT){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)GET_LOCAL_IP,strlen((char*)GET_LOCAL_IP),200);
		//Response for this can be discarded!
		flag.GET_LOCAL_IP_COMM_SENT	=	SET;
		flag.GPRS_COMMAND_ONGOING	=	SET;
		
	}else if(!flag.GPRS_COMMAND_ONGOING	&&	flag.OLD_CONN_SHUTOK	&&	flag.GPRS_AVLBLE	&&	flag.TCP_STACK_INITOK	
	&&	flag.SINGLECON_COMM_OK_RCVD	&&	flag.SETAPN_OK_RCVD	&& flag.GPRSUP_OK_RCVD	&& flag.GET_LOCAL_IP_COMM_SENT	
	&&	!flag.TCP_CONNECTED	&& !flag.TCP_CON_COMM_SENT){
	
		for(char i=0;i<strlen((char*)GPRSCOMM);i++){
			GPRSCOMM[i]	=	0;//clean up buffer
		}
		
		strcat((char*)GPRSCOMM,START_TCP_CONN);
		strcat((char*)GPRSCOMM,(char*)"\"");
		strcat((char*)GPRSCOMM,(char*)GPRS_Details.IP_Address);
		strcat((char*)GPRSCOMM,(char*)"\"");
		strcat((char*)GPRSCOMM,(char*)",");
		strcat((char*)GPRSCOMM,(char*)"\"");
		strcat((char*)GPRSCOMM,(char*)GPRS_Details.Port_Num);
		strcat((char*)GPRSCOMM,(char*)"\"");
		strcat((char*)GPRSCOMM,(char*)"\r\n");
		
		//HAL_Delay(200);
		HAL_UART_Transmit(&huart1,GPRSCOMM,strlen((char*)GPRSCOMM),200);
		flag.GPRS_COMMAND_ONGOING	=	SET;
		flag.TCP_CON_COMM_SENT=SET;
			
	}
}

void GPRS_disconnect(void){

	if(!flag.GPRS_COMMAND_ONGOING	&&	flag.GPRS_AVLBLE	&& flag.TCP_CONNECTED){
	
		HAL_UART_Transmit(&huart1,(unsigned char*)RESET_IP_SESSION,strlen(RESET_IP_SESSION),200);
	
	}
	
}

void GPRS_send(unsigned char message[]){

	if(flag.GPRS_AVLBLE	&& flag.TCP_CONNECTED	&&	!flag.TCP_DATA_COMM_SENT){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)SEND_DATA_TCP,strlen(SEND_DATA_TCP),200);
		flag.TCP_DATA_COMM_SENT	=	SET;
		
	}else if(flag.SIM868E_MSG_RESP_RCVD){
	
		HAL_UART_Transmit(&huart1,message,strlen((char*)message),200);
		HAL_UART_Transmit(&huart1,(unsigned char*)MSG_TERM,strlen(MSG_TERM),200);//send MSG terminator "ctrl+z"	
		flag.SIM868E_MSG_RESP_RCVD	=	RESET;
		
	}
}

