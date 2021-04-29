/*****************************************************************************
 *			File Name		:ffb_debug.c
 *			Version			:V1.0
 *			Create Date	:2017-12-13
 *			Modufy Date	:
 *			Information :
 *****************************************************************************/

#include "tool_block_pool.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ffb_pool_debug.h"
#include "time.h"
#include "string.h"
#include <windows.h>
#include <Mmsystem.h>


#define ffb_info_t

#define ffb_info_t tool_block_pool_info_t 
#define FFB_POOL_API tool_block_pool_api

/*****************************************************************************
 * Private functions
 ****************************************************************************/
 
 /*----------------------------------------------------------------------------
   UI Function
*----------------------------------------------------------------------------*/
static void gotoxy(int x, int y)
{
    static HANDLE h = NULL;  
    if(!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };  
    SetConsoleCursorPosition(h,c);
}

static void cls(void){
	gotoxy(0,0);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
void FFB_DEBUG_ValueShow(FFB_Pool_ID PoolID ,bool ShowFlag){
	ShowFlag=true;
	ffb_info_t *info_debug = (ffb_info_t*)PoolID;
	printf("\n");
	printu();
	printf("isBusy                        =");
	printv(info_debug->isBusy);
	printf("mod                           =");
	printv(info_debug->mod);
	printf("Buffer.pStartPoint            =");
	printx(info_debug->buffer.startPoint);
	printf("Buffer.size                   =");
	printv(info_debug->buffer.size);
	printf("Block.pFlagBlock              =");
	printx(info_debug->block.flagBlock);
	printf("Block.size                    =");
	printv(info_debug->block.size);
	printf("Block.count.blockTotal        =");
	printv(info_debug->block.count.blockTotal);
	printf("Block.count.flagTotal         =");
	printv(info_debug->block.count.flagTotal);
	printf("Block.count.flagUse           =");
	printv(info_debug->block.count.flagUse);
	printf("Block.count.flagStart         =");
	printv(info_debug->block.count.flagStart);
	printf("Block.count.flagCount.value   =");
	printv(info_debug->block.count.flagCount.value);
	printf("Block.count.flagCount.s.flag  =");
	printv(info_debug->block.count.flagCount.s.flag);
	printf("Block.count.flagCount.s.block =");
	printv(info_debug->block.count.flagCount.s.block);
	printf("Block.count.lastFlag.value    =");
	printv(info_debug->block.count.lastFlag.value);
	printf("Block.count.lastFlag.s.flag   =");
	printv(info_debug->block.count.lastFlag.s.flag);
	printf("Block.count.lastFlag.s.block  =");
	printv(info_debug->block.count.lastFlag.s.block);


	printu();
} 

void FFB_DEBUG_LoopTest(FFB_Pool_ID PoolID, uint32_t loopQuantity, uint32_t blockCount){
//system("mode con cols=130 lines=55");
	system("mode con cols=60 lines=30");
	void* ptr[blockCount];
	uint16_t randomNumb[loopQuantity];	
	printv(sizeof(ffb_info_t));
	memset(&ptr, 0x00, sizeof(ptr));

	
	
	uint16_t randomMax = FFB_POOL_API.getFreeCount(PoolID);
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;

	/*
	for(int x=0; x<(randomMax*0.2); x++){
		ptr[x] = FFB_POOL_API.Alloc();
	}
	
	for(int x=0; x<(randomMax*0.73); x++){
		FFB_POOL_API.Alloc();
	}
	/**/
	

	FFB_DEBUG_ValueShow(PoolID, false);
	system("pause");
	system("cls");
	
	uint32_t count_Alloc,	count_Free, count_Error;
	while(1){
		count_Alloc = 0;
		count_Free 	= 0;
		count_Error = 0;
		for(int x=0; x<loopQuantity; x++)
			randomNumb[x] =	(rand()%randomMax);
			
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBeginTime);
		 
		for(int x=0; x<loopQuantity; x++){
			if(ptr[randomNumb[x]] == 0){ 
				ptr[randomNumb[x]] = FFB_POOL_API.alloc(PoolID);
				(ptr[randomNumb[x]] != 0)?count_Alloc+=1:count_Error+=1;
				
			}
			else{
				FFB_POOL_API.free(PoolID, ptr[randomNumb[x]]);
				ptr[randomNumb[x]] = 0;
				count_Free+=1;
			}
		}
		
		QueryPerformanceCounter(&nEndTime);
		time=(double)((nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart)*1000;
		cls();
		printf("Total time:%14fms\n",time);
		printf("Quantity:%16d\n",loopQuantity);
		time = (time*1000*1000)/loopQuantity;
		printf("One time:%16fns\n",time);
		printf("Different:%6d\n",(count_Alloc-count_Free));
		printf("Error    :%6d\n",count_Error);
		float usin = FFB_POOL_API.getUsedCount(PoolID);
		usin = (usin / FFB_POOL_API.getTotalCount(PoolID))*100;
		printf("Using    :%4f%%\n", usin);
		FFB_DEBUG_ValueShow(PoolID, false);
		//system("pause");
	}
}



