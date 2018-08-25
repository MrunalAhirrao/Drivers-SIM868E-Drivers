/*
	This file contains all the vars and function related to flash writing
	
	Currently it is considered that last 2 pages of flash memory are used for different stats storage.
	Stats like:(more can be added)
	1.SIM868E is SET: It indicates that the SIM868_OTF(if not SET before) is called at startup and SIM868 is set
	2.Different params like IP address of server, Port Number, APN
	3.FW version
	
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASHSTORE_H__
#define __FLASHSTORE_H__



#include "stm32f0xx_hal.h"

#define FLASH_STR_START_ADDR 	((uint32_t)0x0800F400)
#define FLASH_STR_END_ADDR 		((uint32_t) 0x0800FBFF)+FLASH_PAGE_SIZE


//FLASH Address of different stats to point to respective vars

#define SIM868E_SET_ADDR  ((uint32_t)0x0800F400)//Single octet
#define IP_ADDR						((uint32_t)0x0800F401)//Currently Considering it as 12 octets.like 35.154.227.1#TODO need to ask how much this can go
#define PORT_NUM_ADDR     ((uint32_t)0x0800F410)//Currently considering 5 octets
#define APN_ADDR          ((uint32_t)0x0800F417)//Currently considering 7 octets

//FLASH Address of different stats to point to respective vars

extern FLASH_EraseInitTypeDef EraseStruct;
extern uint32_t PageError;

/*
	This function will store data to flash. 
	As this is the Flash memory, all pages between FLASH_STR_START_ADDR and FLASH_STR_END_ADDR will be erased, hence always pass
	full array with all values need to be stored.
	
	@param: *ptrDataToBeStored is the pointer to array which needs to be stored.

*/
void StoreToFlash(unsigned char *ptrDataToBeStored);

#endif