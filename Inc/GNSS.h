/*
Header file for SIM868E GNSS module
Author: Mrunal Ahirrao
*/

#include "SIM868E_Generic.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GNSS_H__
#define __GNSS_H__

//GNSS Commands
#define GNSS_PWRUP_COMM "AT+CGNSPWR=1\r\n" //response is OK with below PWRUP response
#define GNSS_PWRDN_COMM "AT+CGNSPWR=0\r\n"//response is OK
#define SET_RMC_SENTCE	"AT+CGNSSEQ=\"RMC\"\r\n"//response is OK
#define SET_GGA_SENTCE	"AT+CGNSSEQ=\"GGA\"\r\n"//response is OK
#define SET_GSA_SENTCE	"AT+CGNSSEQ=\"GSA\"\r\n"//response is OK
#define SET_GSV_SENTCE	"AT+CGNSSEQ=\"GSV\"\r\n"//response is OK
#define GET_GNSS_DATA		"AT+CGNSINF\r\n"//response will be a long string
#define TURNOFF_GNSS_URC	"AT+CGNSURC=0\r\n"//turn off the reporting of GNSS data every GNSS fix, so it will send only when we ask
//GNSS Commands


#define GNSS_PWRUP_RESP "+CGNSPWR: 1"
#define GNSS_DATA_FIX_RESP	"+CGNSINF: 1,1"//this is the repsonse when AT+CGSINF is sent. This is the starting of the string and it is a very long string
#define GNSS_DATA_UNFIX_RESP	"+CGNSINF: 1,0"//this is the repsonse when AT+CGSINF is sent. This is the starting of the string and it is a very long string

typedef struct{
	
	unsigned char UTC_DATE_TIME[19]; 
	unsigned char LAT[11];
	unsigned char LNG[12];
  unsigned char MSL_ALTITUDE[9];	
	unsigned char	SPEED[7];
	unsigned char	COURSE[7];
	unsigned char GPS_SATS_VIEW[3];
	unsigned char	GNSS_SATS_USED[3];
	
}GNSS_Sentence_Typedef;

extern GNSS_Sentence_Typedef GNSS_SENTENCE;

//GNSS functions
void GNSS_PWRUP(void);
void GNSS_PWRDN(void);
void GET_GNSSDATA(void);
void GNSS_SET_RMC_SENTENCE(void);
void GNSS_SET_GGA_SENTENCE(void);
void GNSS_SET_GSA_SENTENCE(void);
void GNSS_SET_GSV_SENTENCE(void);
void TURNOFF_GNSS_FIX_URC(void);
//GNSS fucntions

#endif
