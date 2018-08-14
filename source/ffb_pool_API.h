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

typedef void* FastFlagBlock_Pool_ID;

#define FFB_Pool_ID FastFlagBlock_Pool_ID

typedef enum{
	ffbOK,
	ffbReload,
	ffbAddrError,
	ffbMutex,
	ffbError
}ffbStatus;

typedef struct{
	FFB_Pool_ID	(*reload)(void *pBuffer);
	FFB_Pool_ID	(*init)(void *pBuffer,	uint32_t bufferSize,	uint32_t blockSize);
	uint16_t		(*getUseCount)(FFB_Pool_ID poolID);
	uint16_t		(*getFreeCount)(FFB_Pool_ID poolID);
	uint16_t		(*getTotalCount)(FFB_Pool_ID poolID);
	void* 			(*alloc)(FFB_Pool_ID poolID);
	ffbStatus 	(*free)(FFB_Pool_ID poolID, void* addr);
}FFB_POOL_API_T;

extern const FFB_POOL_API_T FFB_POOL_API;

#endif 
/*****************************************************************************
 * End of file
 ****************************************************************************/

