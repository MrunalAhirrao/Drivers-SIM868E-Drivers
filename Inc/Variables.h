/*
General Global Variables file

Author: Mrunal Ahirrao
*/

//To avoid multiple inclusion
#ifndef __VARIABLES_H__
#define __VARIABLES_H__


//SIM868E status Macros
#define SLEEPING 1 
#define AWAKE 0
//SIM868E status Macros

typedef struct{
	
	//Generic SIM868E related Flags
	unsigned int SIM868E_SLEEPING:1;
	unsigned int SIM868E_GSM_SLEEPING:1;
	unsigned int SIM868E_MSG_RESP_RCVD:1;
	unsigned int SIM868E_TEST_COMM_SENT:1;
	unsigned int SIM868E_COMM_OK:1;
	//Generic SIM868E related Flags
	
	
	//GSM related flags
	unsigned int GSM_COMMAND_ONGOING:1;
	
	//GSM NW responses flags
	unsigned int GSM_NW_NOTREG_TRYING:1;
	unsigned int GSM_NW_NOTREG_NOTTRY:1;
	unsigned int GSM_NW_REG_HOME:1;
	unsigned int GSM_NW_REGTN_DENIED:1;
	unsigned int GSM_NW_NOTREG_UKN:1;
	unsigned int GSM_NW_REG_ROAMNG:1;
	//GSM NW responses flags
	
	unsigned int GSM_CHKNW_COMM_SENT:1;
	unsigned int GSM_GET_IMEI_COMM_SENT:1;
	unsigned int GSM_GET_OPRTR_COMM_SENT:1;
	unsigned int GSM_GETNUM_COMM_SENT:1;
	unsigned int GSM_GETSIG_COMM_SENT:1;
	unsigned int GSM_NW_REGSTRD:1;//this flag indicates NW is registered with home or Roaming 
	unsigned int GSM_IMEI_RCVD:1;
	unsigned int GSM_OPRTR_RCVD:1;
	unsigned int GSM_NUM_RCVD:1;
	unsigned int GSM_SIGQLTY_RCVD:1;
	unsigned int GSM_Init_CPLT:1;
		
	unsigned int GSM_SLEEP_COMM_SENT:1;
	unsigned int GSM_WAKE_COMM_SENT:1;
	
	//SMS related flags
	unsigned int GSM_SMS_SENT:1;
	unsigned int GSM_SMS_RCVD:1;
	unsigned int GSM_SMS_READ_CPLT:1;
	unsigned int GSM_SMSREAD_COMM_SENT:1;
	unsigned int GSM_SMS_COMM_SENT:1;
	unsigned int GSM_SMSDEL_COMM_SENT:1;
	unsigned int GSM_SMS_DEL_CPLT:1;
	//SMS related flags
	
	//GSM related flags
	
	unsigned int TCP_DATA_COMM_SENT:1;
	unsigned int GPRS_COMMAND_ONGOING:1;
	unsigned int GPRS_AVLBLE:1;
	unsigned int GPRS_UP:1;
	unsigned int TCP_CONNECTED:1;
	unsigned int OLD_CONN_SHUTOK:1;
	unsigned int TCP_STACK_INITOK:1;
	unsigned int TCP_SEND_OK:1;
	
	//Flags for specific GPRS commands which responds only with OK
	unsigned int SINGLECON_COMM_SENT:1;
	unsigned int SINGLECON_COMM_OK_RCVD:1;
	
	unsigned int SETAPN_COMM_SENT:1;
	unsigned int SETAPN_OK_RCVD:1;
	
	unsigned int GPRSUP_COMM_SENT:1;
	unsigned int GPRSUP_OK_RCVD:1;
	
	unsigned int TCP_CON_COMM_SENT:1;
	unsigned int TCP_CON_OK_RCVD:1;
	
	unsigned int GET_LOCAL_IP_COMM_SENT:1;//this don't respond with OK but with a local IP address which we don't need currently
	//Flags for specific GPRS commands which responds only with OK
	
	//flags for GNSS
	unsigned int GNSS_COMM_ONGOING:1;
	unsigned int GNSS_FIXED:1;
	unsigned int GNSS_PWRUP:1;
	unsigned int GNSS_PWRUP_COMM_SENT:1;
	unsigned int GNSS_PWRDN_COMM_SENT:1;
	unsigned int GNSS_DATA_COMM_SENT:1;
	unsigned int GNSS_RMC_COMM_SENT:1;
	unsigned int GNSS_GGA_COMM_SENT:1;
	unsigned int GNSS_GSA_COMM_SENT:1;
	unsigned int GNSS_GSV_COMM_SENT:1;
	unsigned int GNSS_RMC_USED:1;
	unsigned int GNSS_GGA_USED:1;
	unsigned int GNSS_GSA_USED:1;
	unsigned int GNSS_GSV_USED:1;
	unsigned int GNSS_FIX_URC_OFF:1;
	unsigned int GNSS_URC_OFF_COMM_SENT:1;
	//flags for GNSS

	//flags related to BLE
	unsigned int BLE_PWRUP:1;
	unsigned int BLE_PWRUP_COMM_SENT:1;
	
	unsigned int BLE_CONNECTED:1;
	
	unsigned int BLE_COMM_ONGOING:1;
	
	unsigned int BLE_EN_SPP_COMM_SENT:1;
	unsigned int BLE_SPP_ENBLD:1;
	
	unsigned int BLE_GATT_REG_COMM_SENT:1;
	unsigned int BLE_GATT_REGSRD:1;
	
	unsigned int BLE_GATT_DEREG_COMM_SENT:1;
	
	unsigned int BLE_GETADDR_COMM_SENT:1;
	unsigned int BLE_GOT_ADDR:1;
	
	unsigned int BLE_INIT_CPLT:1;
	
	unsigned int BLE_DATA_RXD:1;
	
	unsigned int BLE_DATA_SENT:1;
	
	unsigned int BLE_OPENED:1;
	
	unsigned int BLE_STAT_COMM_SENT:1;
	
	unsigned int BLE_DISCON_COMM_SENT:1;
	
	unsigned int BLE_PWRDN_COMM_SENT:1;
	
	unsigned int BLE_SENDIND_COMM_SENT:1;
	//flags related to BLE

}FLAGS;

extern FLAGS flag;

extern	char buffer[2][500]; //two buffers
extern	int bufferCounter[2]; ///two counters
extern	unsigned int uartISR, uartUser, commandFound ; 
extern 	unsigned char GPCntr;//general purpose counter
#endif
