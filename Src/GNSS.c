/*
This file contains the functions for GNSS module of SIM868E

Author: Mrunal Ahirrao.

*/

#include "GNSS.h"
#include "stm32f0xx_hal.h"
#include <string.h>

extern UART_HandleTypeDef huart1;

/*
This will power up the GNSS module
*/
void GNSS_PWRUP(){
	
	if(!flag.GNSS_PWRUP_COMM_SENT	&&	!flag.GNSS_PWRUP){
		
			HAL_UART_Transmit(&huart1,(unsigned char*)GNSS_PWRUP_COMM,strlen(GNSS_PWRUP_COMM),200);
			flag.GNSS_PWRUP_COMM_SENT	=	SET;
	
	}
	
}

void GNSS_PWRDN(){
	
	if(!flag.GNSS_PWRDN_COMM_SENT	&&	flag.GNSS_PWRUP){
	
			HAL_UART_Transmit(&huart1,(unsigned char*)GNSS_PWRDN_COMM,strlen(GNSS_PWRDN_COMM),200);
			flag.GNSS_PWRDN_COMM_SENT	=	SET;
			
	}
	
}


/*
	This function will get GNSS data and fill the struct with new values
*/
void GET_GNSSDATA(){
	
	if(!flag.GNSS_DATA_COMM_SENT){
		
			HAL_UART_Transmit(&huart1,(unsigned char*)GET_GNSS_DATA,strlen(GET_GNSS_DATA),200);
			flag.GNSS_DATA_COMM_SENT	=	SET;
		
	}	
}

//---------------------------------GNSS data format change functions-----------------------------------------------------

/*
	This Function will set the GNSS data format as RMC
*/
void GNSS_SET_RMC_SENTENCE(void){
	
	if(!flag.GNSS_RMC_COMM_SENT	&& !flag.GNSS_RMC_USED){
			
			HAL_UART_Transmit(&huart1,(unsigned char*)SET_RMC_SENTCE,strlen(SET_RMC_SENTCE),200);
			flag.GNSS_RMC_COMM_SENT	=	SET;
		
	}

}	

/*
	This Function will set the GNSS data format as GGA
*/
void GNSS_SET_GGA_SENTENCE(void){
	
	if(!flag.GNSS_GGA_COMM_SENT	&& !flag.GNSS_GGA_USED){
			
			HAL_UART_Transmit(&huart1,(unsigned char*)SET_GGA_SENTCE,strlen(SET_GGA_SENTCE),200);
			flag.GNSS_GGA_COMM_SENT	=	SET;
		
	}

}

/*
	This Function will set the GNSS data format as GSA
*/
void GNSS_SET_GSA_SENTENCE(void){
	
	if(!flag.GNSS_GSA_COMM_SENT	&& !flag.GNSS_GSA_USED){
			
			HAL_UART_Transmit(&huart1,(unsigned char*)SET_GSA_SENTCE,strlen(SET_GSA_SENTCE),200);
			flag.GNSS_GSA_COMM_SENT	=	SET;
		
	}

}

/*
	This Function will set the GNSS data format as GSV
*/
void GNSS_SET_GSV_SENTENCE(void){
	
	if(!flag.GNSS_GSV_COMM_SENT	&& !flag.GNSS_GSV_USED){
			
			HAL_UART_Transmit(&huart1,(unsigned char*)SET_GSV_SENTCE,strlen(SET_GSV_SENTCE),200);
			flag.GNSS_GSV_COMM_SENT	=	SET;
		
	}

}

//---------------------------------GNSS data format change functions-----------------------------------------------------


/*
	This Function will Turnoff GNSS fix URC report, hence GNSS data will only be presented when called GET_GNSSDATA()
*/
void TURNOFF_GNSS_FIX_URC(void){
	
	if(!flag.GNSS_FIX_URC_OFF	&& !flag.GNSS_URC_OFF_COMM_SENT){
		
			HAL_UART_Transmit(&huart1,(unsigned char*)TURNOFF_GNSS_URC,strlen(TURNOFF_GNSS_URC),200);
			flag.GNSS_URC_OFF_COMM_SENT	=	SET;
		
	}		
	
}
