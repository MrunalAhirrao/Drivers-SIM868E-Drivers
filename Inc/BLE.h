/*
	Header for BLE functions,Macros and Variables of SIM868E
	This source is for using SIM868E BLE as a SPP server only!
  
  Author: Mrunal Ahirrao.
*/

#include "SIM868E_Generic.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BLE_H__
#define __BLE_H__

//BLE Commands
#define BLE_PWR_UP						"AT+BTPOWER=1\r\n" //This command will power up the Bluetooth part of SIM868E(OK response)
#define BLE_EN_SPP						"AT+BLESPP=1\r\n"	//This command will setup SPP server profile(OK response)
#define BLE_REG_GATT_SERVER		"AT+BLESREG\r\n" //This command will register GATT Server
#define BLE_DREG_GATT_SERVER	"AT+BLESDREG=1\r\n" //This command will deregister GATT Server of the index 1, considering 1 because we will be registering only one server at a time
#define BLE_GET_ADDR					"AT+BLEADDR?\r\n"	//This command will get BLE address
#define BLE_TEST_PWR					"AT+BTPOWER?\r\n"//This commmand will test if BT is powered up
#define BLE_PWR_DN						"AT+BTPOWER=0\r\n"//This command will power down BLE 
#define BLE_DEREG_SPP_SERVER	"AT+BLESPP=0\r\n"	//This command will deregister SPP server if enabled before. This can be used to disconnect client connected with SPP

/*
This command won't work directly. First needs to get information using BLE status(instead of this above command can be used simpler)
*/
#define BLE_DISCONN						"AT+BLEDISCONN="//This command disconnect the BLE connection. After '=' needs to be added Connection ID
/*
This command won't work directly. First needs to get information using BLE status
*/
#define BLE_STAT							"AT+BLESTATUS?\r\n"//This command will get the BLE status and connections
#define BLE_SPP_SEND_IND			"AT+BLESPPSIND="//This command is used to send data in indication format via SPP server to client. The data needs to be in "" and should be terminated with \r\n
//BLE Commands

//BLE Responses
#define BLE_CONN_NOTI			"+BLESCON: 1"	//This indicates that some App is connectd with BLE, from 44th char is the conn ID
#define BLE_SPP_CONN_NOTI	"+BLESPPCON: 1,"//This indicates that some App is connected with SPP server of BLE. After ',' till CR is remote device BLE address
#define BLE_RX_DATA_NOTI	"+BLESPPWREQ: "//This indcates that some data is received from App via SPP.After this response till ',' is the address of BLE 
//device which sent data, and after "," is the data recevied till CR 
#define BLE_REG_GATT_RESP "+BLESREG: 1"//This indicates that BLE GATT server is registered.
#define BLE_ADDR_RESP			"+BLEADDR: 0,"//This is the response after sending BLE address command. After "," will be the address of the SIM868 BLE
#define BLE_PWR_TEST_RESP	"+BTPOWER: 1"//This indicates that BLE is powered up
#define BLE_DISCONN_RESP	"+BLESCON: 0"//This indcates that BLE disconnected
#define BLE_STAT_RESP			"+BLESTATUS: 1"//This indicates BLE is opened
//#TODO This needs to discussed as number conn ID and below responses will be dependent on services of app 
#define BLE_STAT_CONN_RESP "+BLESTATUS: "//This will be after above response and after ":" will be the connection IDs. For each connection will be separate response. THis connection ID is needed while using disconnect command
#define BLE_SPP_DISCON_RESP "+BLESPPCON: 0"//This indicates device disconnected from SPP server
//BLE Responses

typedef struct{
	unsigned char BLE_RM_ADDR[20];
	unsigned char BLE_RX_DATA[20];
}BLE_DATA;	

extern BLE_DATA BLE_DETAILS;
 

//BLE Functions
void BLE_Init(void);
void BLE_Disconnect(void);
void BLE_POWER_DN(void);
void BLE_SEND_IND(unsigned char message[]);
void BLE_ResetFlags(void);
//BLE Functions

#endif
