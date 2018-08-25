/*
Generic SIM868E Source file containing functions for buffer process and Generic SIM868E functions

Author: Mrunal Ahirrao.

*/

#include "SIM868E_Generic.h"
#include "stm32f0xx_hal.h"
#include "GSM.h"
#include "GPRS.h"
#include "GNSS.h"
#include "BLE.h"
#include <string.h>

SMS_COMMAND_DATA SMS_comm;//SMS related structure which will have data from SMS
GNSS_Sentence_Typedef GNSS_SENTENCE;//GNSS related Structure which will have data related to GNSS
GSM_Data GSM_Details;
BLE_DATA BLE_DETAILS;//BLE related structure 
extern UART_HandleTypeDef huart1;


/*
	Function to send Test command, to test SIM868E is ok with communication
	
*/
void TEST_SIM868E_COMM(){

	HAL_UART_Transmit(&huart1,(unsigned char*)TEST_CMD,strlen(TEST_CMD),200);
	flag.SIM868E_TEST_COMM_SENT	=	SET;
	flag.SIM868E_COMM_OK	=	RESET;
}


/*
** switches the buffers between the ISR and the main loop process
** this technique avoids complicated logic and is simple and powerful
** the ISR and the main process always access different buffers at all times
** no copying or initialization etc. required
*/
void switchBuffers() {
   
		uartISR = !uartISR;
    uartUser = !uartUser;
    bufferCounter[uartISR] = 0; //reset the buffer counter for receiving the UART input
}

/*
** parses the UART data and returns a string
** read thru the buffer and strip off any CR / LF in front or back
** returns a comand string (null terminated) or a null string as output
*/
void getCommand(char* command) {
    int j = 0;
    for(int i=0;i<bufferCounter[uartUser];i++) 
    {
        if (buffer[uartUser][i] == 0x0A || buffer[uartUser][i] == 0x0D); //skip any CRLF before or after
        else 
        {
            command[j] = buffer[uartUser][i];  
            j++;
        }
    }
    command[j] = '\0'; //end of string
}


/*
-------------------------------------------------------------------
Function to sleep SIM868E if awake or to wake SIM868E if sleeping
-------------------------------------------------------------------
*/
void SIM868E_sleep_Wake(){
	HAL_GPIO_WritePin(SIM868E_PWR_PIN_GPIO_Port,SIM868E_PWR_PIN_Pin,1);
	HAL_Delay(5000);
	HAL_GPIO_WritePin(SIM868E_PWR_PIN_GPIO_Port,SIM868E_PWR_PIN_Pin,0);
	
	if(flag.SIM868E_SLEEPING == SLEEPING){
	    flag.SIM868E_SLEEPING = AWAKE;
	}else if(flag.SIM868E_SLEEPING == AWAKE){
	    flag.SIM868E_SLEEPING =	SLEEPING;
	}
}

/*
	This function will return the index of char in string.
	fromwhere should the index from where to start searching
	charToBeFound should be the char of which index is to be found
	*string is the pointer to array from which char needs to be found
*/
unsigned int GetCharIndexof(unsigned int fromwhere,unsigned char charToBeFound,unsigned char* string){
	
	unsigned int i=fromwhere,stringLength;
	
	stringLength=strlen((char*)string);
	
	while(i	<	stringLength){
		if(*string	!= charToBeFound){
			i++;
			string++;
		}else if(*string	== charToBeFound)break;
	};
	
	return(i);
	
}

/*
-------------------------------------------------------------------
Function to process UART buffer and understand SIM868E response
Each response from SIM is parsed and respective buffers and structures are filled and flags are set/reset
-------------------------------------------------------------------
*/
void UART_dataProcess(){

	unsigned char GP_buffer[200]; //GP buffer, this hold the received response from SIM868E

	switchBuffers(); //switch buffers for processing, now we have a command/Or some SIM868E response in the buffer ending with CRLF

	getCommand((char*)GP_buffer);//get command or response of SIM868	
	
	if(GP_buffer[0]!=0x00){//if GP_buffer is not null
	    	
		if(GP_buffer[0]	==	'>'){ //if received '>' which is after sending AT+CMGS or AT+CIPSEND
				
			flag.SIM868E_MSG_RESP_RCVD	=	SET;
			
		}

//-------------------------------------------------------------------------------------------------------------------------------		
		
//----------------------------------parsing for OK Responses--------------------------------------------------------------------
		
		else if(strncmp((char*)GENERIC_SIM868_OK_RESP,(char*)GP_buffer,strlen(GENERIC_SIM868_OK_RESP))==0){//OK response
				
			if(flag.SIM868E_TEST_COMM_SENT){
				
				flag.SIM868E_COMM_OK	=	SET;//first command to be sent to SIM868E to test communication
				flag.SIM868E_TEST_COMM_SENT	=	RESET;
			}
			
			if(flag.GSM_COMMAND_ONGOING){
				
				flag.GSM_COMMAND_ONGOING	=	RESET;
				if(flag.GSM_SMSDEL_COMM_SENT)flag.GSM_SMS_DEL_CPLT	=	SET;
				RESET_SMS_FLAGS();//Reset all SMS flags so that SMS can be sent again
				
			}
				//Setting respective OK response flags to identify this OK response is for which command
				
			else if(flag.GPRS_COMMAND_ONGOING){
						//GPRS related OK reponses
						if(flag.SINGLECON_COMM_SENT){
							flag.SINGLECON_COMM_OK_RCVD	=	SET;
						} 
						if(flag.SETAPN_COMM_SENT){
							flag.SETAPN_OK_RCVD	=	SET;
						}
						if(flag.GPRSUP_COMM_SENT){
							flag.GPRSUP_OK_RCVD	=	SET;
						}
						if(flag.TCP_CON_COMM_SENT){
							flag.TCP_CON_COMM_SENT	=	RESET;
							flag.SINGLECON_COMM_SENT	=	RESET;
							flag.SETAPN_COMM_SENT	=	RESET;
							flag.GPRSUP_COMM_SENT	=	RESET;
							flag.TCP_CON_OK_RCVD	=	SET;
						}
						flag.GPRS_COMMAND_ONGOING	=	RESET;
					}
				//GPRS related OK responses
			
			//GNSS related OK responses
			else if(flag.GNSS_COMM_ONGOING){
					
					if(flag.GNSS_DATA_COMM_SENT){
					
						flag.GNSS_DATA_COMM_SENT	= RESET;
					
					}else if(flag.GNSS_GGA_COMM_SENT){
				//if GNSS data format changed to GGA then reset other flags indicating which format used, so that at at time only one format is selected to be indicated
						flag.GNSS_GGA_COMM_SENT	=	RESET;
						flag.GNSS_GGA_USED	=	SET;
						flag.GNSS_RMC_USED	=	RESET;
						flag.GNSS_GSA_USED	=	RESET;
						flag.GNSS_GSV_USED	=	RESET;
						
					}else if(flag.GNSS_RMC_COMM_SENT){
				//if GNSS data format changed to RMC then reset other flags indicating which format used, so that at at time only one format is selected to be indicated
						flag.GNSS_RMC_COMM_SENT	=	RESET;
						flag.GNSS_RMC_USED	=	SET;
						flag.GNSS_GGA_USED	=	RESET;
						flag.GNSS_GSA_USED	=	RESET;
						flag.GNSS_GSV_USED	=	RESET;
						
					}else if(flag.GNSS_GSA_COMM_SENT){
				//if GNSS data format changed to GSA then reset other flags indicating which format used, so that at at time only one format is selected to be indicated
						flag.GNSS_GSA_COMM_SENT	=	RESET;
						flag.GNSS_GSA_USED	=	SET;
						flag.GNSS_RMC_USED	=	RESET;
						flag.GNSS_GGA_USED	=	RESET;
						flag.GNSS_GSV_USED	=	RESET;
						
					}else if(flag.GNSS_GSV_COMM_SENT){
				//if GNSS data format changed to GSV then reset other flags indicating which format used, so that at at time only one format is selected to be indicated
						flag.GNSS_GSV_COMM_SENT	=	RESET;
						flag.GNSS_GSV_USED	=	SET;
						flag.GNSS_RMC_USED	=	RESET;
						flag.GNSS_GSA_USED	=	RESET;
						flag.GNSS_GGA_USED	=	RESET;
						
					}else if(flag.GNSS_PWRUP_COMM_SENT){
						
						flag.GNSS_PWRUP_COMM_SENT	=	RESET;
							
					}else if(flag.GNSS_PWRDN_COMM_SENT){
					
						flag.GNSS_PWRDN_COMM_SENT	=	RESET;
						flag.GNSS_PWRUP	=	RESET;//GNSS is powered down
					
					}else if(flag.GNSS_URC_OFF_COMM_SENT){
						flag.GNSS_URC_OFF_COMM_SENT	=	RESET;
					}
						flag.GNSS_COMM_ONGOING	=	RESET;
			}				
			//GNSS related OK responses
			
			//BLE related OK responses
			else if(flag.BLE_COMM_ONGOING){
			
				if(flag.BLE_EN_SPP_COMM_SENT){
				//	flag.BLE_EN_SPP_COMM_SENT	=	RESET;
					flag.BLE_SPP_ENBLD	=	SET;
					flag.BLE_INIT_CPLT	=	SET;
					BLE_ResetFlags();

				}
				
				if(flag.BLE_PWRUP_COMM_SENT){
					
					flag.BLE_PWRUP	=	SET;
				//	flag.BLE_PWRUP_COMM_SENT	=	RESET;
			
				}
				
				if(flag.BLE_PWRDN_COMM_SENT){
				
					flag.BLE_PWRUP	=	RESET;
					flag.BLE_INIT_CPLT	=	RESET;
				}
				
				flag.BLE_COMM_ONGOING	=	RESET;
			
			}
			//BLE related OK responses			
			//HAL_Delay(500);

			
//----------------------------------parsing for OK Responses--------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------
			
//--------------------------------Checking Error responses--------------------------------------------------------------- 			
			
			
			}else if(strncmp((char*)SIM868E_ERROR_RESP,(char*)GP_buffer,strlen(SIM868E_ERROR_RESP))==0){//error response
				
				if(flag.GPRS_COMMAND_ONGOING){
				//Reset respective command sent/status flags so these commands can be sent again
				
				if(flag.SINGLECON_COMM_SENT){
					flag.SINGLECON_COMM_SENT	=	RESET;
				} 
				if(flag.SETAPN_COMM_SENT){
					flag.SETAPN_COMM_SENT	=	RESET;
				}
				if(flag.GPRSUP_COMM_SENT){
					flag.GPRSUP_COMM_SENT	=	RESET;
				}		
			
				if(flag.GET_LOCAL_IP_COMM_SENT)flag.GET_LOCAL_IP_COMM_SENT	=	RESET;
				if(flag.GPRSUP_OK_RCVD)flag.GPRSUP_OK_RCVD	=	RESET;
				if(flag.GPRS_AVLBLE)flag.GPRS_AVLBLE	=	RESET;
				if(flag.GPRS_COMMAND_ONGOING)flag.GPRS_COMMAND_ONGOING	=	RESET;
				if(flag.GPRS_UP)flag.GPRS_UP	=	RESET;
				if(flag.TCP_CONNECTED)flag.TCP_CONNECTED	=	RESET;
				if(flag.TCP_CON_COMM_SENT)flag.TCP_CON_COMM_SENT	=	RESET;
				if(flag.TCP_CON_OK_RCVD)flag.TCP_CON_OK_RCVD	=	RESET;
				if(flag.TCP_DATA_COMM_SENT)flag.TCP_DATA_COMM_SENT	=	RESET;
				if(flag.TCP_SEND_OK)flag.TCP_SEND_OK	=	RESET;
				if(flag.TCP_STACK_INITOK)flag.TCP_STACK_INITOK	=	RESET;
				if(flag.SINGLECON_COMM_OK_RCVD)flag.SINGLECON_COMM_OK_RCVD	=	RESET;
				if(flag.SETAPN_OK_RCVD)flag.SETAPN_OK_RCVD	=	RESET;
				if(flag.OLD_CONN_SHUTOK)flag.OLD_CONN_SHUTOK	=	RESET;
			}
				
			}

			
//--------------------------------Checking Error responses--------------------------------------------------------------- 			

//-------------------------------------------------------------------------------------------------------------------------------


//-------------------------------check if this is the GSM related response------------------------------------------------    			

			else if(strncmp(GSM_NW_NOTREG_D_RESP,(char*)GP_buffer,strlen(GSM_NW_NOTREG_D_RESP))==0){//denied NW registration
			
				flag.GSM_CHKNW_COMM_SENT	=	RESET;
				flag.GSM_NW_REGTN_DENIED	=	SET;
				flag.GSM_COMMAND_ONGOING = RESET;
				
			}else if(strncmp((char*)GSM_NW_NOTREG_T_RESP,(char*)GP_buffer,strlen(GSM_NW_NOTREG_T_RESP))==0){//not registerd but trying
			
				flag.GSM_CHKNW_COMM_SENT	=	RESET;
				flag.GSM_NW_NOTREG_TRYING	=	SET;
				flag.GSM_NW_REGSTRD	=	RESET;//indicates GSM is not registerd with any NW
				flag.GSM_COMMAND_ONGOING = RESET;
				
			}else if(strncmp((char*)GSM_NW_NOTREG_N_RESP,(char*)GP_buffer,strlen(GSM_NW_NOTREG_N_RESP))==0){//not registerd and not trying
			
				flag.GSM_CHKNW_COMM_SENT	=	RESET;
				flag.GSM_NW_NOTREG_NOTTRY	=	SET;
				flag.GSM_NW_REGSTRD	=	RESET;//indicates GSM is not registerd with any NW
				flag.GSM_COMMAND_ONGOING = RESET;
			
			}else if(strncmp((char*)GSM_NW_REG_H_RESP,(char*)GP_buffer,strlen(GSM_NW_REG_H_RESP))==0){//registerd with home NW
			
				flag.GSM_NW_REG_HOME	=	SET;
				flag.GSM_NW_REGSTRD	=	SET;//indicates registerd with some NW
				flag.GSM_COMMAND_ONGOING = RESET;
				
			}else if(strncmp((char*)GSM_NW_NOTREG_U_RESP,(char*)GP_buffer,strlen(GSM_NW_NOTREG_U_RESP))==0){//not registerd with unknown response
			
				flag.GSM_CHKNW_COMM_SENT	=	RESET;
				flag.GSM_NW_NOTREG_UKN	=	RESET;// indicates some unknown response received from tower to SIM868E
				flag.GSM_NW_REGSTRD	=	RESET;//indicates GSM is not registerd with any NW
				flag.GSM_COMMAND_ONGOING = RESET;
				
			}else if(strncmp((char*)GSM_NW_REG_R_RESP,(char*)GP_buffer,strlen(GSM_NW_REG_R_RESP))==0){//registered and roaming

				flag.GSM_NW_REG_ROAMNG	=	SET;
				flag.GSM_NW_REGSTRD		=	SET;//indicates registerd with some NW
				flag.GSM_COMMAND_ONGOING = RESET;
				
			}else if(strncmp((char*)SIG_QLTY_RESP,(char*)GP_buffer,strlen(SIG_QLTY_RESP))==0){
			
				//Copying the Signal quality causes hardfault interrupt(program crash!) need to find something on this further.
				//currently leaving this, as it is not that important.
				//strncpy((char*)GSM_Details.GSM_SIG_QLY,(char*)GP_buffer+5,9);// copy signal quality data
				flag.GSM_SIGQLTY_RCVD	=	SET;
				flag.GSM_COMMAND_ONGOING	=	RESET;
				flag.GSM_Init_CPLT	=	SET;
				RESET_GSM_INIT_FLAGS();// This is the last GSM init Command hence reset all init flags, so that next time GSM_init() should work
				
			}else if(strncmp((char*)OPR_NAME_RESP,(char*)GP_buffer,strlen(OPR_NAME_RESP))==0){
			
				strncpy((char*)GSM_Details.SIM_OPR_NAME,(char*)GP_buffer+12,GetCharIndexof(12,'"',GP_buffer));//copy SIM operator name
				flag.GSM_OPRTR_RCVD	=	SET;
				flag.GSM_COMMAND_ONGOING	=	RESET;
				
			}else if(flag.GSM_GET_IMEI_COMM_SENT	&& !flag.GET_LOCAL_IP_COMM_SENT){//if some response is recived and IMEI comm sent but local IP comm not sent then this  maybe IMEI command response
				
				strncpy((char*)GSM_Details.IMEI_Num,(char*)GP_buffer,strlen((char*)GP_buffer));//copy IMEI number
				flag.GSM_COMMAND_ONGOING	=	RESET;
				flag.GSM_IMEI_RCVD	=	SET;
				
			}
			
//-------------------------------check if this is the GSM related response------------------------------------------------    			

//-------------------------------------------------------------------------------------------------------------------------------
			
//-------------------------------check if this is the BLE related responses------------------------------------------------------			

//		else if(strncmp(BLE_REG_GATT_RESP,(char*)GP_buffer,strlen(BLE_REG_GATT_RESP))==0){
//		
//			flag.BLE_INIT_CPLT	=	SET;
//			BLE_ResetFlags();
//			
//		}else if(strncmp(BLE_DREG_GATT_SERVER,(char*)GP_buffer,strlen(BLE_DREG_GATT_SERVER))==0){
//		
//			flag.BLE_GATT_REGSRD	=	RESET;
//			flag.BLE_GATT_DEREG_COMM_SENT	=	RESET;
//			
//		}
			else if(strncmp(BLE_ADDR_RESP,(char*)GP_buffer,strlen(BLE_ADDR_RESP))==0){
		
			flag.BLE_GOT_ADDR	=	SET;
			flag.BLE_GETADDR_COMM_SENT	=	RESET;
			
		}else if(strncmp(BLE_STAT_RESP,(char*)GP_buffer,strlen(BLE_STAT_RESP))==0){
		
			flag.BLE_OPENED	=	SET;
			flag.BLE_STAT_COMM_SENT	=	RESET;
		
		}else if(strncmp(BLE_SPP_CONN_NOTI,(char*)GP_buffer,strlen(BLE_SPP_CONN_NOTI))==0){
		
			flag.BLE_CONNECTED	=	SET;
			strncpy((char*)BLE_DETAILS.BLE_RM_ADDR,(char*)GP_buffer+14,GetCharIndexof(14,0x0D,GP_buffer));//copy connected remote BLE address 
			
//		}else if(strncmp(BLE_CONN_NOTI,(char*)GP_buffer,strlen(BLE_SPP_CONN_NOTI))==0){
//		
//			flag.BLE_CONNECTED	=	SET;
//			strncpy((char*)BLE_CONN_DETAILS.BLE_CONN_ID,(char*)GP_buffer+44,GetCharIndexof(14,0x0D,GP_buffer));//copy Conn_ID(needed while disconnecting)
//		
//		}
//		else if(strncmp(BLE_DISCONN_RESP,(char*)GP_buffer,strlen(BLE_DISCONN_RESP))==0){
//		
//			flag.BLE_CONNECTED	=	RESET;
//			flag.BLE_DISCON_COMM_SENT	=	RESET;
//		
//		}
		}else if(strncmp(BLE_SPP_DISCON_RESP,(char*)GP_buffer,strlen(BLE_SPP_DISCON_RESP))==0){
		
			flag.BLE_CONNECTED	=	RESET;
			flag.BLE_DISCON_COMM_SENT	=	RESET;
		
		}else if(strncmp(BLE_RX_DATA_NOTI,(char*)GP_buffer,strlen(BLE_RX_DATA_NOTI))==0){
		
			flag.BLE_DATA_RXD	=	SET;//some data received via BLE from connected device
			strncpy((char*)BLE_DETAILS.BLE_RX_DATA,(char*)GP_buffer+31,GetCharIndexof(31,0x0D,GP_buffer));//Copy received Data
		
		}
				
			
//-------------------------------check if this is the BLE related responses------------------------------------------------------			

			
//-------------------------------------------------------------------------------------------------------------------------------
			
			
//-------------------------------check if this is the SMS related response------------------------------------------------    			
			
			else if(strncmp((char*)SMS_RCVD_NOTI,(char*)GP_buffer,strlen(SMS_RCVD_NOTI))==0){
				
				flag.GSM_SMS_RCVD	=	SET;
					
			}else if(strncmp((char*)SMS_READ_RESP,(char*)GP_buffer,strlen(SMS_READ_RESP))==0){// process the SMS read response
				
				strncpy((char*)SMS_comm.NUM_FROM_SMS_RCVD,(char*)GP_buffer+24,10);//copy 10 chars from 24 of SIM868 buffer which would be mobile number of sender of SMS
				
			}else if(strncmp((char*)SMS_SENT_RESP,(char*)GP_buffer,strlen(SMS_SENT_RESP))==0){
			
				flag.GSM_SMS_SENT	=	SET;
				
			}else if(flag.GSM_SMS_RCVD){
				
				if(strncmp((char*)SMS_comm.NUM_FROM_SMS_RCVD,(char*)NUM,strlen(NUM)) == 0){//check received number is same as that stored
					
					//copying SMS msg from 64 char till last \r which will be the SMS command
					strncpy((char*)SMS_comm.RCVD_SMS_COMMAND,(char*)GP_buffer+64,strlen((char*)GP_buffer));//Get SMS command from SIM868 SMS response
					flag.GSM_SMS_READ_CPLT = SET; //if sender number matched and SMS command from message is read then set the GSM_SMS_READ_CPLT flag
					
					GSM_Delete_SMS();
					
				}else {
				
					GSM_Delete_SMS();
				
				}
				flag.GSM_SMS_RCVD	=	RESET;// RESET the SMS recived flag
			}				
//--------------------------------------------------check if this is the SMS related response----------------------------------    

//-------------------------------------------------------------------------------------------------------------------------------
						
//--------------------------------------------------Check if these are GPRS related responses------------------------------------	

			//#TODO need to find the response of GPRS when its disconnected from server or due to loss of Network
			else if(strncmp((char*)CHK_GPRS_STAT_RESP,(char*)GP_buffer,strlen(CHK_GPRS_STAT_RESP))==0){
				
				flag.GPRS_AVLBLE	=	SET;
				flag.GPRS_COMMAND_ONGOING	=	RESET;
				
			}else if(strncmp((char*)CHK_GPRS_STAT_RESP_NO,(char*)GP_buffer,strlen(CHK_GPRS_STAT_RESP_NO))==0){
			
				flag.GPRS_AVLBLE	=	RESET;
				flag.GPRS_COMMAND_ONGOING	=	RESET;
			
			}else if(strncmp((char*)TCP_CONNECTED_RESP,(char*)GP_buffer,strlen(TCP_CONNECTED_RESP))==0){
				
				flag.TCP_CONNECTED	=	SET;
				flag.GPRS_COMMAND_ONGOING	=	RESET;

			}else if(strncmp((char*)CIP_SHUT_RESP,(char*)GP_buffer,strlen(CIP_SHUT_RESP))==0){
			
				flag.OLD_CONN_SHUTOK	=	SET;
				flag.TCP_CONNECTED	=	RESET;//TCP disconnected(if it was connected before)
				flag.GPRS_COMMAND_ONGOING	=	RESET;
			
			}else if(strncmp((char*)CIP_STAT_RESP,(char*)GP_buffer,strlen(CIP_STAT_RESP))==0){

				flag.TCP_STACK_INITOK	=	SET;
				flag.GPRS_COMMAND_ONGOING	=	RESET;
				
			}else if(strncmp((char*)TCP_SEND_RESP,(char*)GP_buffer,strlen(TCP_SEND_RESP))==0){
				
				flag.TCP_SEND_OK	=	SET;
				flag.TCP_DATA_COMM_SENT	=	RESET;
				flag.GPRS_COMMAND_ONGOING	=	RESET;
				
			}				
//--------------------------------------------------Check if these are GPRS related responses------------------------------------	

//-------------------------------------------------------------------------------------------------------------------------------


//--------------------------------------------------Check if these are GNSS related response-------------------------------------

			else if(strncmp((char*)GNSS_PWRUP_RESP,(char*)GP_buffer,strlen(GNSS_PWRUP_RESP))==0){
				
				flag.GNSS_PWRUP	=	SET;//GNSS POWERED up
			
			}else if(strncmp((char*)GNSS_DATA_FIX_RESP,(char*)GP_buffer,strlen(GNSS_DATA_FIX_RESP))==0){
			
				flag.GNSS_FIXED	=	SET;// GNSS_DATA_RESP fixed
				
				//GNSS Fix found now fill up new GNSS data in struct
				
				strncpy((char*)GNSS_SENTENCE.UTC_DATE_TIME,(char*)GP_buffer+14,strlen((char*)18));//extract UTC which will be from 14th char and max 18 chars
			
				strncpy((char*)GNSS_SENTENCE.LAT,(char*)GP_buffer+33,strlen((char*)10));//extract LAT which will be from 33th char and max 10 chars
				
				strncpy((char*)GNSS_SENTENCE.LNG,(char*)GP_buffer+44,strlen((char*)11));//extract LNG which will be from 44th char and max 11 chars
				
				strncpy((char*)GNSS_SENTENCE.MSL_ALTITUDE,(char*)GP_buffer+57,strlen((char*)8));//extract Altitude which will be from 57th char and max 8 chars
				
				strncpy((char*)GNSS_SENTENCE.SPEED,(char*)GP_buffer+65,strlen((char*)6));//extract speed which will be from 65th char and max 6 chars
				
				strncpy((char*)GNSS_SENTENCE.COURSE,(char*)GP_buffer+73,strlen((char*)6));//extract Course which will be from 73th char and max 6 chars
				
				//TODO
				//strncpy((char*)GNSS_SENTENCE.GPS_SATS_VIEW,(char*)GP_buffer+73,strlen((char*)6));//extract Course which will be from 73th char and max 6 chars
				
			//	strncpy((char*)GNSS_SENTENCE.GNSS_SATS_USED,(char*)GP_buffer+73,strlen((char*)6));//extract Course which will be from 73th char and max 6 chars
				
			}else if(strncmp((char*)GNSS_DATA_UNFIX_RESP,(char*)GP_buffer,strlen(GNSS_DATA_UNFIX_RESP))==0){
			
				flag.GNSS_FIXED	=	RESET;// GNSS_DATA_RESP fixed
			
			}
//--------------------------------------------------Check if these are GNSS related response-------------------------------------

			
//--------------------if response was unknown then reset things------------------------------------------//			
//Unknown response is not possible but some of its response is discarded, like Local IP hence for it this is used			
			if(flag.GPRS_COMMAND_ONGOING)flag.GPRS_COMMAND_ONGOING	=	RESET;
//--------------------if response was unknown then reset things------------------------------------------//	
	}
	
	commandFound=0;
	
}


/*
	This function will "SET" the SIM868E to needed settings. Like Echo off and setting device name for Bluetooth, etc.
	This will be called only onetime(ie first time when system starts and its status will be stored to flash and will be read always at startup)
*/

void SIM868_OTF(){
}
