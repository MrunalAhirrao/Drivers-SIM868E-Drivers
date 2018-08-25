/*
Functions and variables,constants for GPRS 
*/

#include "SIM868E_Generic.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPRS_H__
#define __GPRS_H__



//Macros for GPRS commands
#define CHECK_GPRS_STAT "AT+CGATT?\r\n"//command to test GPRS status
#define SET_APN "AT+CSTT=" //command to set APN. After "=" needs to be added the APN of SIM with \r\n will response with OK
#define BRINGUP_WIRELESS_CONN "AT+CIICR\r\n" //command to enable Wireless conn will response with OK
#define START_TCP_CONN "AT+CIPSTART= \"TCP\"," //command to start TCP connection. AFter "," needs to be added the "IP Address","Port Number"
#define SEND_DATA_TCP "AT+CIPSEND\r\n" //command to send data over TCP, will response with '>' which needs to be terminated with 26 after message
#define RESET_IP_SESSION "AT+CIPSHUT\r\n" //command to reset IP session if any
#define SET_SINGLE_CONN "AT+CIPMUX=0\r\n" //command to set single connection only,will response with OK
#define CHK_CIP_STAT "AT+CIPSTATUS\r\n"//command to check CIP status of GPRS
#define GET_LOCAL_IP "AT+CIFSR\r\n"//command to get Local IP address of SIM868E
//Macros for GPRS commands


//Macros for GPRS responses
#define CHK_GPRS_STAT_RESP "+CGATT: 1"	//it means GPRS is available
#define CHK_GPRS_STAT_RESP_NO "+CGATT: 0"	//it means GPRS is not available
#define TCP_CONNECTED_RESP "CONNECT OK"
#define CIP_SHUT_RESP "SHUT OK"
#define TCP_SEND_RESP "SEND OK"//after this Server response will be received with CRLF!
#define CIP_STAT_RESP "STATE: IP INITIAL"
//Macros for GPRS responses


//GPRS vars
typedef struct {
	unsigned char IP_Address[15];
	unsigned char Port_Num[6];
	unsigned char APN[8];
}GPRS_Params;

extern GPRS_Params GPRS_Details;

extern unsigned char RCVD_SERVER_RESP[200];
//GPRS vars


//GPRS functions
void GPRS_init(void);
//void GPRS_connect(void);
void GPRS_disconnect(void);
void GPRS_send(unsigned char message[]);
void GPRS_RX_formatdata(char *buffer);
void GPRS_TX_formatdata(char *buffer);
//GPRS functions

#endif