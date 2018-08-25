/*
Source file for GSM functions of SIM868E

Author: Mrunal Ahirrao

*/

#include "GSM.h"
#include "stm32f0xx_hal.h"
#include <string.h>

extern UART_HandleTypeDef huart1;
FLAGS flag;


/*
This function will check if SIM868 is registered with NW and get details like IMEI_NUMBER, GET_SIM_OPERATOR NAME,GET SIM number,and signal quality
*/
void GSM_init(){
	
	if(!flag.GSM_COMMAND_ONGOING	&& !flag.GSM_CHKNW_COMM_SENT){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)CHK_NW_REGSTRD,strlen(CHK_NW_REGSTRD),200);
		flag.GSM_CHKNW_COMM_SENT	=	SET;
		flag.GSM_COMMAND_ONGOING = SET;
		
	}else if(!flag.GSM_COMMAND_ONGOING && !flag.GSM_GET_IMEI_COMM_SENT	&&	flag.GSM_CHKNW_COMM_SENT	&&	flag.GSM_NW_REGSTRD){	
		
		HAL_UART_Transmit(&huart1,(unsigned char*)GET_IMEI_NUMBER,strlen(GET_IMEI_NUMBER),200);
		flag.GSM_GET_IMEI_COMM_SENT	=	SET;
		flag.GSM_COMMAND_ONGOING = SET;
		
	}else if(!flag.GSM_COMMAND_ONGOING	&&	!flag.GSM_GET_OPRTR_COMM_SENT	&&	flag.GSM_GET_IMEI_COMM_SENT	&&	flag.GSM_IMEI_RCVD){ 
	
		HAL_UART_Transmit(&huart1,(unsigned char*)GET_SIMOPERATOR_NAME,strlen(GET_SIMOPERATOR_NAME),200);
	  flag.GSM_GET_OPRTR_COMM_SENT	=	SET;
		flag.GSM_COMMAND_ONGOING = SET;
		
	}/*This command was not working on SIM868E hence removed temporarily 
	else if(!flag.GSM_COMMAND_ONGOING	&&	!flag.GSM_GETNUM_COMM_SENT	&&	
		flag.GSM_GET_OPRTR_COMM_SENT	&&	flag.GSM_GET_IMEI_COMM_SENT	&&	flag.GSM_IMEI_RCVD	&& flag.GSM_OPRTR_RCVD){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)GET_SIM_NUMBER,strlen(GET_SIM_NUMBER),200);
		flag.GSM_GETNUM_COMM_SENT	=	SET;
		
	}*/else if(!flag.GSM_COMMAND_ONGOING	&&	!flag.GSM_GETSIG_COMM_SENT	/*&&	flag.GSM_GETNUM_COMM_SENT*/	&&	
		flag.GSM_GET_OPRTR_COMM_SENT	&&	flag.GSM_GET_IMEI_COMM_SENT	&&	flag.GSM_IMEI_RCVD	&& flag.GSM_OPRTR_RCVD	
	&&	!flag.GSM_SIGQLTY_RCVD){
		
		HAL_UART_Transmit(&huart1,(unsigned char*)GET_GSM_SIG_QLTY,strlen(GET_GSM_SIG_QLTY),200);  
		flag.GSM_GETSIG_COMM_SENT	=	SET;
		flag.GSM_COMMAND_ONGOING = SET;

	}
}


/*
	Function to reset GSM initialization flags, without this the GSM_init() won't work
*/

void RESET_GSM_INIT_FLAGS(){

		flag.GSM_CHKNW_COMM_SENT	=	RESET;
		flag.GSM_GETNUM_COMM_SENT	=	RESET;
		flag.GSM_GETSIG_COMM_SENT	=	RESET;
		flag.GSM_GET_IMEI_COMM_SENT	=	RESET;
		flag.GSM_GET_OPRTR_COMM_SENT	=	RESET;
		flag.GSM_IMEI_RCVD	=	RESET;
		flag.GSM_NUM_RCVD	=	RESET;
		//flag.GSM_NW_REGSTRD	=	RESET;
		flag.GSM_OPRTR_RCVD	=	RESET;
		flag.GSM_SIGQLTY_RCVD	=	RESET;
	
		flag.GSM_NW_NOTREG_NOTTRY	=	RESET;
		flag.GSM_NW_NOTREG_TRYING	=	RESET;
		flag.GSM_NW_NOTREG_UKN	=	RESET;
		flag.GSM_NW_REGTN_DENIED	=	RESET;
		flag.GSM_NW_REG_HOME	=	RESET;
		flag.GSM_NW_REG_ROAMNG	=	RESET;
	
}



/*
This function will sleep the GSM Part
*/
void GSM_sleep(void){
	HAL_UART_Transmit(&huart1,(unsigned char*)SLEEP_GSM,sizeof(SLEEP_GSM),200);
}

/*
This function will wake slept GSM Part
*/
void GSM_wake(void){
	HAL_UART_Transmit(&huart1,(unsigned char*)WAKE_GSM,sizeof(WAKE_GSM),200);
}


/*
This function will send SMS command to SIM868E
*/
void GSM_sendSMSComm(void){
	
	char SMS_SEND_COMM[24],i=0;
	while(i<sizeof(SMS_SEND_COMM)){
		SMS_SEND_COMM[i]=0;//cleanup array
		i++;
	};
	strncpy(SMS_SEND_COMM,SEND_SMS,sizeof(SEND_SMS));
	strncat(SMS_SEND_COMM,NUM,sizeof(NUM)); // copy concatenated String of "AT+CMGS="NUM"" and copy to SMS_SEND_COMM
	strncat(SMS_SEND_COMM,"\r\n",2);//terminate with CR and LF
	
	HAL_UART_Transmit(&huart1,(unsigned char*)SMS_SEND_COMM,strlen(SMS_SEND_COMM),200);//send the SMS_SEND_COMM string with CR LF
	flag.GSM_COMMAND_ONGOING = SET;
	flag.GSM_SMS_COMM_SENT	=	SET;
}

/*
This function will send the SMS message to number sent in GSM_sendSMSComm()
*/
void GSM_sendSMS(unsigned char message[]){

	HAL_UART_Transmit(&huart1,message,strlen((const char*)message),200);//send the SMS_SEND_COMM string with CR LF
	HAL_UART_Transmit(&huart1,(unsigned char*)"\r\n",2,200);
	HAL_UART_Transmit(&huart1,(unsigned char*)MSG_TERM,strlen(MSG_TERM),200);//send MSG terminator "ctrl+z"	
  flag.GSM_COMMAND_ONGOING = RESET;
	flag.GSM_SMS_COMM_SENT	=	SET;
	flag.SIM868E_MSG_RESP_RCVD	=	RESET;
}

/*
This function will send the SMS message reading command to SIM868E and the SMS will be read and stored in SMS_Command_DATA struct
*/
void GSM_readSMS(void){

	HAL_UART_Transmit(&huart1,(unsigned char*)READ_SMS,strlen(READ_SMS),200); 	
	flag.GSM_SMSREAD_COMM_SENT	=	SET;
	
}

/*
This function will send the SMS message Delete command to SIM868E
*/
void GSM_Delete_SMS(void){

	HAL_UART_Transmit(&huart1,(unsigned char*)DELETE_SMS,strlen(DELETE_SMS),200);
	flag.GSM_COMMAND_ONGOING = SET;
	flag.GSM_SMSDEL_COMM_SENT	=	SET;
}

/*
	This function will reset all SMS related flags
*/
void RESET_SMS_FLAGS(){

		flag.GSM_SMSREAD_COMM_SENT	=	RESET;
		flag.GSM_SMS_COMM_SENT	=	RESET;
		flag.GSM_SMS_READ_CPLT	=	RESET;
		flag.GSM_SMS_SENT	=	RESET;
		flag.GSM_SMS_DEL_CPLT	=	RESET;

}
