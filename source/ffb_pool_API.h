/*****************************************************************************
 *			File Name		:ffb_API.h
 *			Version			:V1.0
 *			Create Date	:2017-12-06
 *			Modufy Date	:
 *			Information :
 *****************************************************************************/
#ifndef H_FFB_POOL_API
#define H_FFB_POOL_API 
#include "stdint.h"

typedef void* FFB_Pool_ID;
/*
#define FFB_Pool_ID FastFlagBlock_Pool_ID
*/
typedef enum{
	ffbOK,
	ffbReload,
	ffbAddrError,
	ffbMutex,
	ffbError
}ffbStatus;

typedef struct{
	FFB_Pool_ID	(*Reload)(void *pBuffer);
	FFB_Pool_ID	(*Init)(void *pBuffer,	uint32_t bufferSize,	uint32_t blockSize);
	uint16_t		(*GetUseCount)(FFB_Pool_ID poolID);
	uint16_t		(*GetFreeCount)(FFB_Pool_ID poolID);
	uint16_t		(*GetTotalCount)(FFB_Pool_ID poolID);
	void* 			(*Alloc)(FFB_Pool_ID poolID);
	ffbStatus 	(*Free)(FFB_Pool_ID poolID, void* addr);
}FFB_POOL_API_T;

extern FFB_POOL_API_T FFB_POOL_API;

#endif 
/*****************************************************************************
 * End of file
 ****************************************************************************/

