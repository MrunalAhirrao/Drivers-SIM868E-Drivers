/*
Generic SIM868E related functions and constants
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SIM868E_GENERIC_H__
#define __SIM868E_GENERIC_H__


#include "Variables.h"

void SIM868E_sleep(void);
void SIM868E_wakeup(void);
void SIM868_OTF(void);

#define SIM868E_SIMCARD_NOTRDY_RESP "+CPIN: NOT READY"// this is the response when SIM card was removed!and GPRS not active
#define SIM868E_PWR_DN_RESP "NORMAL POWER DOWN"
#define SAVE_PARAMS_TO_SIME "AT&W\r\n" //command to store params in SIM868E memory
#define GENERIC_SIM868_OK_RESP "OK" 
#define DATA_SEND_RESP '>' //this will be the response after sending the AT+CMGS="NUM"\r\n or AT+CIPSEND. After this data needs to be sent ending with 0x1A
#define MSG_TERM "\032\r\n"//Ctrl+Z for terminating the message in SMS and GPRS
#define SIM868E_ERROR_RESP "ERROR" // this can be received when SIM868 is not a mood to start!
#define TEST_CMD	"AT\r\n"//This is to test if SIM868E is awake and listening. This shall response with OK

void SIM868E_sleep_Wake(void);

void TEST_SIM868E_COMM(void);

void UART_dataProcess(void);

#endif