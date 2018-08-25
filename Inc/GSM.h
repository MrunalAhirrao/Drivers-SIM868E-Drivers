/*
GSM functions and variables and macros for SIM868E
Author: Mrunal Ahirrao
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GSM_H__
#define __GSM_H__


#include "SIM868E_Generic.h"

#define NUM "\"8830134432\""
//Macros for GSM commands

#define GET_GSM_SIG_QLTY "AT+CSQ\r\n" //command to get signal quality report
#define CHK_NW_REGSTRD	"AT+CREG?\r\n"//command to check SIM868E registered with NW
#define GET_SIM_NUMBER "AT+CNUM\r\n" //command to get Subscriber number(not working only OK is received!) 
#define DELETE_SMS "AT+CMGDA=\"DEL ALL\"\r\n" //Command to delete all SMS
#define SET_SMS_FORMAT "AT+CMGF=1\r\n"//commmand to set SMS format to text
#define READ_SMS "AT+CMGR=1\r\n" //command to read SMS always at 1, because SMS will deleted after reading
#define SEND_SMS "AT+CMGS=" //command to send SMS. After "=" needs to be adding number to whom SMS needs to be sent and ended with \r\n
#define GET_SIMOPERATOR_NAME "AT+COPS?\r\n" //command to get operator name
#define GET_IMEI_NUMBER "AT+CGSN\r\n" 
#define SLEEP_GSM "AT+CFUN=4\r\n" //command to sleep GSM part
#define WAKE_GSM "AT+CFUN=1\r\n" //command to wake GSM part

//Macros for GSM commands

//Macros for SMS Responses(all these responses are after CR LF, hence at 2nd char in buffer)

#define SMS_SENT_RESP "+CMGS" //response after SMS sent
#define SMS_RCVD_NOTI "+CMTI: \"SM\","//response after SMS is received, data after "," will be the index of SMS stored at 14 th #define
#define SMS_READ_RESP "+CMGR" //response after sending AT+CMGR=<index>. This is the staring string receivd after CR LF. AFter 25 #defines the sender's number is found.After 64 #defines the message can be found till CR

//Macros for SMS Responses

//Macros for GSM related Responses

#define SIG_QLTY_RESP "+CSQ:" //response after sending Signal qlty command. The report will be at 5th char of buffer till CR
#define OPR_NAME_RESP "+COPS:" //response after sending AT+COPS? command.Name of operator will be from 12 th char to " char
#define GSM_NW_NOTREG_T_RESP 	"+CREG: 0,2"//response indicating GSM is not registered with NW, but trying to register
#define GSM_NW_NOTREG_N_RESP 	"+CREG: 0,0"//response indicating GSM is not registered with NW and not trying to register
#define GSM_NW_REG_H_RESP 		"+CREG: 0,1"//response indicating GSM is registered with Home NW(don't know what Home is! as given in command manual)
#define GSM_NW_NOTREG_D_RESP 	"+CREG: 0,3"//response indicating GSM is not registered with NW and registration denied 
#define GSM_NW_NOTREG_U_RESP 	"+CREG: 0,4"//response unknown and maybe indicate GSM is not registered!
#define GSM_NW_REG_R_RESP 		"+CREG: 0,5"//response indicating GSM is registered with NW and roaming

//#TODO using CREG we can get location information like 2 byte location area code and 2 byte cell id
//To use  it one needs to change the command like AT+CREG=2\r\n and in above responses of CREG instead of 0 we will get 2 and 
//and location area code from 12 th char to 15 th char and from 19 th char to 22 nd char will the cell ID

//Macros for GSM related responses


//GSM Functions
void GSM_init(void);
void RESET_GSM_INIT_FLAGS(void);
void GSM_sleep(void);
void GSM_wake(void);
void GSM_sendSMS(unsigned char message[]);
void GSM_readSMS(void);
void GSM_getlocation(void);
void GSM_Delete_SMS(void);
void GSM_sendSMSComm(void);
void RESET_SMS_FLAGS(void);
//GSM Functions

typedef struct{

	unsigned char IMEI_Num[15];
	//unsigned char SIM_Num[11];//SIM868E don't support this, only OK is responsed!(atleast in my case)
	unsigned char SIM_OPR_NAME[100];
	//unsigned char GSM_SIG_QLY[6];//Currently I have removed this.

}GSM_Data;

extern GSM_Data GSM_Details;

typedef struct{
	
	unsigned char	NUM_FROM_SMS_RCVD[11];
	unsigned char RCVD_SMS_COMMAND[30];

}SMS_COMMAND_DATA;

extern SMS_COMMAND_DATA SMS_comm;

#endif
