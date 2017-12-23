/*****************************************************************************
 *			File Name		:ffb.c
 *			Version			:V1.0
 *			Create Date	:2017-12-04
 *			Modufy Date	:2017-12-13
 *			Information :
 *****************************************************************************/
#include "ffb_pool.h" 
#include "ffb_pool_API.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define FFB_POOL_FoolproofEnable 0

#if FFB_POOL_FoolproofEnable
#define FFB_POOL_Foolproof(value) 	if(poolID == 0)return value
#else
#define FFB_POOL_Foolproof(value)
#endif

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/
 
#define FFB_POOL_IsBlockFlagFree(ffb_pool_info ,blockNum, flagNum) ((ffb_pool_info->Block.pFlagBlock[blockNum]&(1<<flagNum)) != 0)?false:true

#define FFB_POOL_IsBlockFull(ffb_pool_info, blockNum) (ffb_pool_info->Block.pFlagBlock[blockNum] == 0xFF)?true:false

#define FFB_POOL_SetFlag(ffb_pool_info, blockNum, flagNum, enable)				\
(enable)?																																	\
ffb_pool_info->Block.pFlagBlock[blockNum] |=  (1<<flagNum):								\
ffb_pool_info->Block.pFlagBlock[blockNum] &= ~(1<<flagNum)								

#define FFB_POOL_GetPoolBlockAddr(ffb_pool_info, value) (void*)((uint32_t)ffb_pool_info + (ffb_pool_info->Block.size*value))

#define FFB_POOL_GetMutex(ffb_pool_info) (ffb_pool_info->isBusy == 0)?true:false

#define FFB_POOL_SetMutex(ffb_pool_info, enable) ffb_pool_info->isBusy = enable

#define FFB_POOL_FlagAdd(ffb_pool_info)																															\
if(ffb_pool_info->Block.Count.flagCount.value >= (ffb_pool_info->Block.Count.flagTotal.value-1))		\
	ffb_pool_info->Block.Count.flagCount.value = ffb_pool_info->Block.Count.flagStart;								\
else																																																\
	ffb_pool_info->Block.Count.flagCount.value++

#define FFB_POOL_BlockFlagAdd(ffb_pool_info)																												\
if(ffb_pool_info->Block.Count.flagCount.S.block >= (ffb_pool_info->Block.Count.blockTotal-1))				\
	ffb_pool_info->Block.Count.flagCount.value = ffb_pool_info->Block.Count.flagStart;								\
else{																																																\
	ffb_pool_info->Block.Count.flagCount.S.block++;																										\
	ffb_pool_info->Block.Count.flagCount.S.flag = 0;																									\
}	

#define FFB_POOL_GetPoolBlockFlag(ffb_pool_info, ptr)													\
(ffb_pool_info->mod == 0)?																										\
(((uint32_t)ptr - (uint32_t)ffb_pool_info)/ffb_pool_info->Block.size):				\
(((uint32_t)ptr - (uint32_t)ffb_pool_info) >> ffb_pool_info->mod)	


/*****************************************************************************
 * Public functions
 ****************************************************************************/
 
/*----------------------------------------------------------------------------
   FFB_POOL_Reload
*----------------------------------------------------------------------------*/
FFB_Pool_ID FFB_POOL_Reload(void *pBuffer){
	ffb_info_t *pTmp = (ffb_info_t*)pBuffer;
	
	if(pTmp->Buffer.pStartPoint == pBuffer){
		return (FFB_Pool_ID)pBuffer;
	}
	else
		return (void*)0x00000000;
}

/*----------------------------------------------------------------------------
   FFB_POOL_Init
*----------------------------------------------------------------------------*/
FFB_Pool_ID FFB_POOL_Init(void *pBuffer,	uint32_t bufferSize,	uint32_t blockSize){
	int32_t temp, cnt;
	ffb_info_t *info = (ffb_info_t*)pBuffer;
	
	memset(pBuffer, 0x00, sizeof(ffb_info_t));
	info->Buffer.pStartPoint	= pBuffer;
	info->Buffer.size					=	bufferSize;
	info->Block.size					= blockSize;
		
	/*	block	*/
	info->Block.Count.flagTotal.value	 = (bufferSize / blockSize);
	info->Block.pFlagBlock = (uint8_t*)((uint32_t)pBuffer + sizeof(ffb_info_t));
	info->Block.Count.blockTotal = ((info->Block.Count.flagTotal.value + 7) / 8);
	info->Block.Count.flagStart = ((sizeof(ffb_info_t) + info->Block.Count.blockTotal + blockSize - 1) / blockSize);	
	info->Block.Count.lastFlag.value = 0;
	memset(info->Block.pFlagBlock, 0x00, info->Block.Count.blockTotal);
	
	for(cnt = 0; cnt < info->Block.Count.flagStart; cnt++){
		uint16_t cntA = (cnt/8);
		uint16_t cntB = (cnt%8);
		FFB_POOL_SetFlag(info, info->Block.pFlagBlock[cntA], cntB, true);
	}
	info->Block.Count.flagUse = info->Block.Count.flagStart;
	info->Block.Count.flagCount.value = (info->Block.Count.flagStart - 1);
		
	if(info->Block.Count.flagTotal.S.flag != 0){
		for(cnt = info->Block.Count.flagTotal.S.flag; cnt < 8; cnt++)
			FFB_POOL_SetFlag(info, info->Block.Count.flagTotal.S.block, cnt, true);
	}
		
	for(cnt=0, temp=0; cnt<16; cnt++){
		if(((blockSize>>cnt)&1) == 1){
			temp++;
		}
			
		if(temp>1)break;
	}
		
	if(temp == 1){
		for(cnt = 1; cnt<16; cnt++){
			if(((blockSize>>cnt)&1) == 1){
				info->mod = cnt;
				break;
			}
		}
	}
	
	return (FFB_Pool_ID)pBuffer;
}

/*----------------------------------------------------------------------------
   FFB_POOL_GetUseCount
*----------------------------------------------------------------------------*/
uint16_t	FFB_POOL_GetUseCount(FFB_Pool_ID poolID){
	FFB_POOL_Foolproof(0);
	return ((ffb_info_t*)poolID)->Block.Count.flagUse;
}

/*----------------------------------------------------------------------------
   FFB_POOL_GetFreeCount
*----------------------------------------------------------------------------*/
uint16_t	FFB_POOL_GetFreeCount(FFB_Pool_ID poolID){
	FFB_POOL_Foolproof(0);
	return (((ffb_info_t*)poolID)->Block.Count.flagTotal.value - 
					((ffb_info_t*)poolID)->Block.Count.flagUse);
}

/*----------------------------------------------------------------------------
   FFB_POOL_GetTotalCount
*----------------------------------------------------------------------------*/
uint16_t	FFB_POOL_GetTotalCount(FFB_Pool_ID poolID){
	FFB_POOL_Foolproof(0);
	return ((ffb_info_t*)poolID)->Block.Count.flagTotal.value;
}

/*----------------------------------------------------------------------------
   FFB_POOL_Alloc
*----------------------------------------------------------------------------*/
void* FFB_POOL_Alloc(FFB_Pool_ID poolID){
	ffb_info_t *info = (ffb_info_t*)poolID;
	void* result;
	int i, cnt;
	
	
	if(!FFB_POOL_GetMutex(info))
		return 0;
	FFB_POOL_SetMutex(info, true);
	
	if(info->Block.Count.flagUse == info->Block.Count.flagTotal.value) 
		return 0;
	
		
	//If Last free flag is not empty.
	if(info->Block.Count.lastFlag.value != 0){
		result = FFB_POOL_GetPoolBlockAddr(info, info->Block.Count.lastFlag.value);
		info->Block.Count.flagUse+=1;
		FFB_POOL_SetFlag(info, info->Block.Count.lastFlag.S.block, info->Block.Count.lastFlag.S.flag, true);
		info->Block.Count.lastFlag.value = 0;
		FFB_POOL_SetMutex(info, false);
		return result;
	}
	
	//If next space is free will run into here.
	FFB_POOL_FlagAdd(info);
	if(FFB_POOL_IsBlockFlagFree(info ,info->Block.Count.flagCount.S.block, info->Block.Count.flagCount.S.flag)){
		result = FFB_POOL_GetPoolBlockAddr(info, info->Block.Count.flagCount.value);
		info->Block.Count.flagUse+=1;
		FFB_POOL_SetFlag(info, info->Block.Count.flagCount.S.block, info->Block.Count.flagCount.S.flag, true);
		FFB_POOL_SetMutex(info, false);
		return result;
	}
	
	//If next space is using will run into here.
	for(i=0; i<info->Block.Count.blockTotal; i+=1){
		if(!FFB_POOL_IsBlockFull(info, info->Block.Count.flagCount.S.block)){
			for(cnt=0; cnt<8; cnt++){
				if(FFB_POOL_IsBlockFlagFree(info ,info->Block.Count.flagCount.S.block, cnt)){
					info->Block.Count.flagCount.S.flag = cnt;
					break;
				}
			}
			
			result = FFB_POOL_GetPoolBlockAddr(info, info->Block.Count.flagCount.value);
			info->Block.Count.flagUse+=1;
			FFB_POOL_SetFlag(info, info->Block.Count.flagCount.S.block, info->Block.Count.flagCount.S.flag, true);
			FFB_POOL_SetMutex(info, false);
			return result;
		}
		FFB_POOL_BlockFlagAdd(info);
	}
	
	/* 
	for(i=0; i < info->Block.Count.flagTotal.value; i++){
		info->Block.Count.flagCount.value = i;
		if(FFB_POOL_IsBlockFlagFree(info->Block.Count.flagCount.S.block, info->Block.Count.flagCount.S.flag)){
			result = FFB_POOL_GetPoolBlockAddr(info, info->Block.Count.flagCount.value);
			info->Block.Count.flagUse+=1;
			FFB_POOL_SetFlag(info, info->Block.Count.flagCount.S.block, info->Block.Count.flagCount.S.flag, true);
			FFB_POOL_Mutex(false);
			return result;
		}
	} */ 
	//Never run in here
	FFB_POOL_SetMutex(info, false);
	return result;
}

/*----------------------------------------------------------------------------
   FFB_POOL_Free
*----------------------------------------------------------------------------*/
ffbStatus FFB_POOL_Free(FFB_Pool_ID poolID, void* addr){
	ffb_info_t *info = (ffb_info_t*)poolID;
	FFB_POOL_Foolproof(ffbError);
	
	if(!FFB_POOL_GetMutex(info))
		return ffbMutex;
		
	FFB_POOL_SetMutex(info, true);
	
	if(((uint32_t)addr < (uint32_t)info) | ((uint32_t)addr > ((uint32_t)info + info->Buffer.size))){
		FFB_POOL_SetMutex(info, false);
		return ffbAddrError;
	}
	
	info->Block.Count.lastFlag.value = FFB_POOL_GetPoolBlockFlag(info, addr);
	
	if(FFB_POOL_IsBlockFlagFree(info ,info->Block.Count.lastFlag.S.block, info->Block.Count.lastFlag.S.flag)){
		FFB_POOL_SetMutex(info, false);
		return ffbError;
	}
	
	info->Block.Count.flagUse-=1;
	FFB_POOL_SetFlag(info, info->Block.Count.lastFlag.S.block, info->Block.Count.lastFlag.S.flag, false);
	FFB_POOL_SetMutex(info, false);
	return ffbOK;
}

/*----------------------------------------------------------------------------
   FFB_POOL_API
*----------------------------------------------------------------------------*/
FFB_POOL_API_T FFB_POOL_API = {
	.Reload						=	FFB_POOL_Reload,
	.Init							=	FFB_POOL_Init,
	.GetUseCount			=	FFB_POOL_GetUseCount,
	.GetFreeCount			=	FFB_POOL_GetFreeCount,
	.GetTotalCount		=	FFB_POOL_GetTotalCount,
	.Alloc						=	FFB_POOL_Alloc,
	.Free							=	FFB_POOL_Free
};

/*****************************************************************************
 * End of file
 ****************************************************************************/
